using Pkg

# Ensure we load the in-repo ROS2.jl package, not a different ROS2 on the global LOAD_PATH.
Pkg.activate(joinpath(@__DIR__, "julia"))
Pkg.resolve()
Pkg.instantiate()

using ROS2

function main()
    node = Node("julia_pubsub_test")

    pub = create_publisher(node, StringMsg, "chatter")
    sub = create_subscription(node, StringMsg, "chatter")

    exec = create_executor(node.ctx)
    add_subscription(exec, sub)

    # Give the middleware a moment to match endpoints.
    for _ in 1:5
        spin_once(exec, 50)
    end

    publish(pub, StringMsg("hello from julia"))

    received = nothing
    for _ in 1:100
        spin_once(exec, 50)
        msg = take(sub)
        if msg !== nothing
            received = msg
            break
        end
    end

    if received === nothing
        error("did not receive message")
    end

    println("received: ", received.data)
end

main()
