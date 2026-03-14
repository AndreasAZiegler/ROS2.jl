export ExecutorHandle, create_executor, add_subscription, spin_once, spin, is_ready

"""
    ExecutorHandle

Owns the context to ensure the context outlives the executor.
"""
struct ExecutorHandle
    ctx::Context
    impl::Executor
end

function create_executor(ctx::Context)
    impl = Executor(ctx)  # CxxWrap constructor
    return ExecutorHandle(ctx, impl)
end

function add_subscription(exec::ExecutorHandle, sub::SubscriptionHandle)
    # CxxWrap exports methods as Julia functions: add_subscription(exec_impl, sub_impl).
    ROS2.add_subscription(exec.impl, sub.impl)
    return nothing
end

spin_once(exec::ExecutorHandle, timeout_ms::Integer) = ROS2.spin_once(exec.impl, Int(timeout_ms))

is_ready(exec::ExecutorHandle, sub::SubscriptionHandle) = ROS2.is_ready(exec.impl, sub.impl)

"""
    spin(exec::ExecutorHandle; timeout_ms::Integer=100)

Run an infinite loop calling `spin_once`.
"""
function spin(exec::ExecutorHandle; timeout_ms::Integer=100)
    while true
        spin_once(exec, timeout_ms)
    end
end
