using Pkg

Pkg.activate(joinpath(@__DIR__, "julia"))
Pkg.resolve()
Pkg.instantiate()

using ROS2

function main()
    node = Node("julia_action_test")

    # Requires the C++ shim to have been built with the corresponding action headers
    # available at compile time. Currently the shim provides a small lookup table.
    ts = get_action_typesupport("example_interfaces/action/Fibonacci")

    srv = create_action_server(node, ts, "fibonacci")
    cli = create_action_client(node, ts, "fibonacci")

    println("created action server and client for example_interfaces/Fibonacci on /fibonacci")

    # Give discovery some time.
    exec = create_executor(node.ctx)
    for _ in 1:20
        spin_once(exec, 50)
        sleep(0.01)
    end

    println("OK: init + discovery step complete")
    println("TODO: end-to-end goal/feedback/result requires generated Julia bindings for the action's rosidl_generator_c message structs")
end

main()
