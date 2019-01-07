#pragma once
#include <stdio.h>
#include <memory>
#include <gsl/pointers>
#include <cc/null.hpp>
#include <optional>
#include <cc/span.hpp>
#include <cc/cast.hpp>
#include <cc/scope.hpp>
#include <gsl/pointers>



namespace cc
{

using std::optional;
using gsl::not_null;

enum class FilePos : int
{
	Head = SEEK_SET,
	Tail = SEEK_END,
	Current = SEEK_CUR,
};

class file final
{
	template<typename T>
	using basic_string_view = std::basic_string_view<T>;
	using string_view = std::string_view;

public:
	static file open (not_null<const char*> file_name, not_null<const char*> mode) noexcept
	{
		return ::fopen (file_name, mode);
	}

	FILE* get () noexcept { return fp_.get (); }
	auto ftell () noexcept { return ::ftell (get ()); }

	int fseek (FilePos origin, long offset = 0) noexcept
	{
		return ::fseek (get (), offset, static_cast<int>(origin));
	}

	size_t fwrite (span<const char> view) noexcept
	{
		return ::fwrite (view.data (), sizeof (char), static_cast<size_t> (view.size()), get ());
	}

	auto fread (span<char> view) noexcept
	{
		return ::fread (view.data (), sizeof (char), static_cast<size_t> (view.size ()), get ());
	}

	template<typename ... Args>
	int fprintf (const char * fmt, Args && ... args) noexcept
	{
		return ::fprintf (get (), fmt,  std::forward<Args> (args)...);
	}

	explicit operator bool () const noexcept { return fp_ != nullptr; }

	[[nodiscard]]
	inline static std::optional<std::string> read_all (not_null<const char*> path) noexcept;

	inline static bool override (not_null<const char*> path, gsl::span<const char> data) noexcept;
	inline static bool append (not_null<const char*> path, string_view data) noexcept;

private:
	file (FILE * fp) noexcept : fp_ (fp) { }
private:
	struct fclose_deleter { void operator () (FILE* fp) { fclose (fp); } };
	std::unique_ptr<FILE, fclose_deleter> fp_ = nullptr;
};


[[nodiscard]]
std::optional<std::string> file::read_all (not_null<const char*> path) noexcept
{
	auto fp = file::open (path, not_null("rb"));
	if (!fp)
	{
		return {};
	}

	if (fp.fseek (FilePos::Tail) != 0)
	{
		return {};
	}

	const auto len = fp.ftell ();

	if (len == -1L)
	{
		return {};
	}

	const auto u_len = static_cast<size_t> (len);

	if (fp.fseek (FilePos::Head) != 0)
	{
		return {};
	}

	std::string ret;
	ret.resize (u_len);

	if (fp.fread (ret) != u_len)
	{
		return std::nullopt;
	}

	return std::move (ret);
}

bool file::override(not_null<const char *> path, gsl::span<const char> data) noexcept
{
	auto fp = file::open (path, not_null("wb"));
	if (!fp)
	{
		return false;
	}
	const auto len = fp.fwrite (data);

	if (len != static_cast<size_t> (data.size ()))
	{
		return false;
	}

	return true;
}

[[nodiscard]]
bool file::append(not_null<const char *> path, string_view data) noexcept
{
	auto fp = file::open (path, not_null("ab+"));
	if (!fp)
	{
		return false;
	}
	const auto len = fp.fwrite (data);

	if (len != data.size ())
	{
		return false;
	}

	return true;
}

}
