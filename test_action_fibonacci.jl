using Pkg

Pkg.activate(joinpath(@__DIR__, "julia"))
Pkg.resolve()
Pkg.instantiate()

using ROS2

function fixed_goal_id()
    # 16 bytes; arbitrary but consistent.
    return UInt8[0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,0x10]
end

function main()
    node = Node("julia_action_fibonacci_test")
    ts = get_action_typesupport("example_interfaces/action/Fibonacci")

    srv = create_action_server(node, ts, "fibonacci")
    cli = create_action_client(node, ts, "fibonacci")

    gid = fixed_goal_id()

    # Client: send goal request
    goal_req = ROS2.FibonacciSendGoalRequest()
    ROS2.set_order(goal_req, Int32(7))
    ROS2.set_goal_id_bytes(goal_req, pointer(gid))

    seq_goal = send_goal_request(cli, ROS2.ros_message(goal_req))
    println("sent goal request seq=", seq_goal)

    # Server: take goal request and respond
    hdr_goal = ROS2.RequestHeader()
    srv_goal_req_in = ROS2.FibonacciSendGoalRequest()
    accepted = false
    for _ in 1:200
        if take_goal_request(srv, hdr_goal, ROS2.ros_message(srv_goal_req_in))
            goal_resp = ROS2.FibonacciSendGoalResponse()
            ROS2.set_accepted(goal_resp, true)
            send_goal_response(srv, hdr_goal, ROS2.ros_message(goal_resp))
            accepted = true
            break
        end
        sleep(0.01)
    end
    accepted || error("server did not receive goal request")

    # Client: take goal response
    hdr_goal_resp = ROS2.RequestHeader()
    goal_resp_in = ROS2.FibonacciSendGoalResponse()
    got_goal_resp = false
    for _ in 1:200
        if take_goal_response(cli, hdr_goal_resp, ROS2.ros_message(goal_resp_in))
            got_goal_resp = true
            break
        end
        sleep(0.01)
    end
    got_goal_resp || error("client did not receive goal response")

    # Client: send result request
    result_req = ROS2.FibonacciGetResultRequest()
    ROS2.set_goal_id_bytes(result_req, pointer(gid))
    seq_res = send_result_request(cli, ROS2.ros_message(result_req))
    println("sent result request seq=", seq_res)

    # Server: take result request and respond with computed fibonacci
    hdr_res = ROS2.RequestHeader()
    srv_res_req_in = ROS2.FibonacciGetResultRequest()
    responded = false
    for _ in 1:200
        if take_result_request(srv, hdr_res, ROS2.ros_message(srv_res_req_in))
            res_resp = ROS2.FibonacciGetResultResponse()
            ROS2.set_status(res_resp, Int8(0))
            ROS2.set_sequence_fibonacci(res_resp, Int32(7))
            send_result_response(srv, hdr_res, ROS2.ros_message(res_resp))
            responded = true
            break
        end
        sleep(0.01)
    end
    responded || error("server did not receive result request")

    # Client: take result response and validate
    hdr_res_resp = ROS2.RequestHeader()
    res_resp_in = ROS2.FibonacciGetResultResponse()
    got_res = false
    for _ in 1:200
        if take_result_response(cli, hdr_res_resp, ROS2.ros_message(res_resp_in))
            got_res = true
            break
        end
        sleep(0.01)
    end
    got_res || error("client did not receive result response")

    seq = Int32[]
    len = ROS2.sequence_length(res_resp_in)
    for i in 0:(len-1)
        push!(seq, ROS2.sequence_at(res_resp_in, i))
    end

    println("received sequence: ", seq)

    expected = Int32[0,1,1,2,3,5,8,13]
    seq == expected || error("unexpected fibonacci sequence")

    println("OK")
end

main()
