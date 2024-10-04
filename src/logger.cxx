module;
#include <format>
#include <string>
#include <chrono>
#include <source_location>
#include <iostream>
#include <fstream>
export module simple_logger;

namespace simple_logger
{
    #define FOREACH_LOG_TYPE(func) \
        func(debug) \
        func(info) \
        func(warning) \
        func(error)
    
    export enum class log_type : std::uint8_t
    {
    #define DEFINE_TYPE(name) name,
        FOREACH_LOG_TYPE(DEFINE_TYPE)
    #undef DEFINE_TYPE
    };

    std::string get_type_name(log_type type)
    {
        switch (type)
        {
        #define CASE(name) case ::simple_logger::log_type::name: return #name;
            FOREACH_LOG_TYPE(CASE)
        #undef CASE
        }
        return "";
    }


    constexpr std::string get_color(log_type type)
    {
#if defined(__APPLE__) || defined(__linux__)
        constexpr std::string color[] = {"", "", "\033[33m", "\033[31m"};
        return color[static_cast<std::int32_t>(type)];
#else
        return "";
#endif
    }

    template<class T>
    class format_location_wrapper
    {
        std::source_location loc_;
        T fmt_str_;
    public:
        template<class U> requires std::constructible_from<T, U>
        consteval format_location_wrapper(U&& in_fmt_str, std::source_location called_loc = std::source_location::current()) : fmt_str_(std::forward<U>(in_fmt_str)), loc_(std::move(called_loc)) {}
        virtual ~format_location_wrapper() = default;

        constexpr const std::source_location& get_source_location() const { return loc_; }
        constexpr const T& get_format_string() const { return fmt_str_; }
    };

    log_type g_log_level = log_type::debug;
    std::ofstream g_out_file("test.log");

    /// @brief log with less importance will be ignored
    /// @param new_level new minimum log level
    export void set_log_level(log_type new_level) { g_log_level = new_level; }

    export template<typename... Args>
    void log(log_type type, format_location_wrapper<std::format_string<Args...>> fmt_wrapper, Args &&... args)
    {
        if(type < g_log_level) return;
        const auto& loc = fmt_wrapper.get_source_location();
        const auto& fmt = fmt_wrapper.get_format_string();
        const auto msg = std::format("{}:{} [{}] ", loc.file_name(), loc.line(), get_type_name(type));
        std::cout << get_color(type) + msg + std::vformat(fmt.get(), std::make_format_args(args...)) + '\n';
        if(g_out_file)
            g_out_file << msg + std::vformat(fmt.get(), std::make_format_args(args...)) + '\n';
    }
} // namespace simple_logger
