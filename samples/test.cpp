import simple_logger;

int main()
{
    simple_logger::log(simple_logger::log_type::warning, "hello {}", "warning");
    simple_logger::log(simple_logger::log_type::error, "hello error {}", 404);
    return 0;
}