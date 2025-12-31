#pragma once

#include <vector>
#include "unordered_dense.h"
#include "dcon_generated_ids.hpp"
#include "constants.hpp"

namespace sys {
struct state; // this is here simply to declare the state struct in a very general location

inline float red_from_int(uint32_t v) {
	return float(v & 0xFF) / 255.0f;
}
inline float green_from_int(uint32_t v) {
	return float((v >> 8) & 0xFF) / 255.0f;
}
inline float blue_from_int(uint32_t v) {
	return float((v >> 16) & 0xFF) / 255.0f;
}
inline uint32_t pack_color(float r, float g, float b) {
	return ((uint32_t(r * 255.0f) & 0xFF) << 0) | ((uint32_t(g * 255.0f) & 0xFF) << 8) | ((uint32_t(b * 255.0f) & 0xFF) << 16);
}

inline int32_t int_red_from_int(uint32_t v) {
	return int32_t(v & 0xFF);
}
inline int32_t int_green_from_int(uint32_t v) {
	return int32_t((v >> 8) & 0xFF);
}
inline int32_t int_blue_from_int(uint32_t v) {
	return int32_t((v >> 16) & 0xFF);
}
inline uint32_t pack_color(int32_t r, int32_t g, int32_t b) {
	return ((uint32_t(r) & 0xFF) << 0) | ((uint32_t(g) & 0xFF) << 8) | ((uint32_t(b) & 0xFF) << 16);
}

struct hsv {
	float h;
	float s;
	float v;
};

inline hsv rgb_to_hsv(uint32_t v) {
	auto r = red_from_int(v);
	auto g = green_from_int(v);
	auto b = blue_from_int(v);

	auto cmin = std::min(r, std::min(g, b));
	auto cmax = std::max(r, std::max(g, b));
	auto delta = cmax - cmin;

	float h = 0.0f;
	if(delta == 0.0f) {
		h = 0.0f;
	} else if(cmax == r) {
		h = 60.0f * (fmod((g - b) / delta, 6.0f));
	} else if(cmax == g) {
		h = 60.0f * ((b - r) / delta + 2.0f);
	} else /*if(cmax == b)*/ {
		h = 60.0f * ((r - g) / delta + 4.0f);
	}

	return hsv{
		h,
		cmax == 0.0f ? 0.0f : delta / cmax,
		cmax
	};
}

inline uint32_t hsv_to_rgb(hsv v) {
	auto c = v.v * v.s;
	auto x = c * (1.0f - std::abs(fmod(v.h / 60.0f, 2.0f) - 1.0f));
	auto m = v.v - c;
	float r = 0.0f; float g = 0.0f; float b = 0.0f;
	if(0.0f <= v.h && v.h < 60.0f) {
		r = c; g = x; b = 0.0f;
	} else if(60.0f <= v.h && v.h < 120.0f) {
		r = x; g = c; b = 0.0f;
	} else if(120.0f <= v.h && v.h < 180.0f) {
		r = 0.0f; g = c; b = x;
	} else if(180.0f <= v.h && v.h < 240.0f) {
		r = 0.0f; g = x; b = c;
	} else if(240.0f <= v.h && v.h < 300.0f) {
		r = x; g = 0.0f; b = c;
	} else /* if(300.0f <= v.h && v.h < 360.0f) */ {
		r = c; g = 0.0f; b = x;
	}
	return pack_color(
		std::clamp(r + m, 0.0f, 255.0f),
		std::clamp(g + m, 0.0f, 255.0f),
		std::clamp(b + m, 0.0f, 255.0f)
	);
}

} // namespace sys

template<typename value_type, typename tag_type, typename allocator = std::allocator<value_type>>
class tagged_vector {
private:
	std::vector<value_type, allocator> storage;

public:
	using public_value_type = value_type;
	using public_tag_type = tag_type;

	tagged_vector() { }
	tagged_vector(tagged_vector<value_type, tag_type, allocator> const& other) noexcept : storage(other.storage) { }
	tagged_vector(tagged_vector<value_type, tag_type, allocator>&& other) noexcept : storage(std::move(other.storage)) { }
	tagged_vector(size_t size) {
		storage.resize(size);
	}

	tagged_vector& operator=(tagged_vector<value_type, tag_type, allocator> const& other) noexcept {
		storage = other.storage;
		return *this;
	}
	tagged_vector& operator=(tagged_vector<value_type, tag_type, allocator>&& other) noexcept {
		storage = std::move(other.storage);
		return *this;
	}
	value_type const& operator[](tag_type t) const {
		assert(size_t(t.index()) < storage.size());
		return *(storage.data() + t.index());
	}
	value_type& operator[](tag_type t) {
		assert(size_t(t.index()) < storage.size());
		return *(storage.data() + t.index());
	}
	template<typename... T>
	tag_type emplace_back(T&&... ts) {
		storage.emplace_back(std::forward<T>(ts)...);
		return tag_type(typename tag_type::value_base_t(storage.size() - 1));
	}
	value_type& safe_get(tag_type t) {
		if(t.index() >= std::ssize(storage))
			storage.resize(t.index() + 1);
		return storage[t.index()];
	}
	auto data() const {
		return storage.data();
	}
	auto data() {
		return storage.data();
	}
	auto array() const {
		return storage.data();
	}
	auto array() {
		return storage.data();
	}
	auto begin() const {
		return storage.begin();
	}
	auto end() const {
		return storage.end();
	}
	auto begin() {
		return storage.begin();
	}
	auto end() {
		return storage.end();
	}
	auto size() const {
		return storage.size();
	}
	auto ssize() const {
		return std::ssize(storage);
	}
	void resize(size_t size) {
		storage.resize(size);
	}
	void reserve(size_t size) {
		storage.reserve(size);
	}
	void pop_back() {
		storage.pop_back();
	}
	tag_type push_back(value_type const& v) {
		storage.push_back(v);
		return tag_type(typename tag_type::value_base_t(storage.size() - 1));
	}
	tag_type push_back(value_type&& v) {
		storage.push_back(std::move(v));
		return tag_type(typename tag_type::value_base_t(storage.size() - 1));
	}
	value_type& back() {
		return storage.back();
	}
	value_type const& back() const {
		return storage.back();
	}
};


namespace sys {

struct checksum_key {
	static constexpr uint32_t key_size = 64;
	uint8_t key[key_size] = { 0 };

	bool is_equal(const checksum_key& a) noexcept {
		for(size_t i = 0; i < key_size; i++)
			if(key[i] != a.key[i])
				return false;
		return true;
	}

	std::string_view to_string_view() noexcept {
		return std::string_view{ reinterpret_cast<const char*>(&key[0]), key_size };
	}
	std::string to_string() noexcept {
		return std::string(to_string_view());
	}
	const char* to_char() noexcept {
		return reinterpret_cast<const char*>(&key[0]);
	}
};
static_assert(sizeof(checksum_key) == sizeof(checksum_key::key));

template<typename IT, typename PTR, typename CMP>
void merge_sort_interior(IT first, IT end, PTR buffer_start, PTR buffer_end, CMP const& cmp) noexcept {
	auto rng_size = end - first;
	if(rng_size == 0) {
		return;
	} else if(rng_size == 1) {
		*first = *buffer_start;
		return;
	} else if(rng_size == 2) {
		if(cmp(*buffer_start, *(buffer_start + 1))) {
			*first = *buffer_start;
			*(first + 1) = *(buffer_start + 1);
		} else {
			*first = *(buffer_start + 1);
			*(first + 1) = *buffer_start;
		}
		return;
	}

	auto half_way = first + rng_size / 2;
	auto b_half_way = buffer_start + rng_size / 2;

	merge_sort_interior(buffer_start, b_half_way, first, half_way, cmp);
	merge_sort_interior(b_half_way, buffer_end, half_way, end, cmp);

	auto temp_index = b_half_way;
	while(temp_index != buffer_end && buffer_start != b_half_way) {
		if(cmp(*temp_index, *buffer_start)) {
			*first = *temp_index;
			++temp_index;
			++first;
		} else {
			*first = *buffer_start;
			++buffer_start;
			++first;
		}
	}
	if(temp_index == buffer_end) {
		std::copy_n(buffer_start, (end - first), first);
	} else if(buffer_start == b_half_way) {
		std::copy_n(temp_index, (end - first), first);
	}
}

template<typename IT, typename CMP>
void merge_sort(IT first, IT end, CMP const& cmp) noexcept {
	auto rng_size = end - first;
	if(rng_size == 0 || rng_size == 1) {
		return;
	} else if(rng_size == 2) {
		if(!cmp(*first, *(first + 1))) {
			std::swap(*first, *(first + 1));
		}
		return;
	}

	using element_type = std::remove_cvref_t<decltype(*first)>;
	auto buffer = new element_type[rng_size];
	std::copy_n(first, rng_size, buffer);
	merge_sort_interior(first, end, buffer, buffer + rng_size, cmp);
	delete[] buffer;
}

struct aui_pending_bytes {
	char const* data = nullptr;
	size_t size = 0;
};

struct wire {
	struct position {
		int16_t x;
		int16_t y;
	};
	std::optional<position> start;
	std::optional<position> end;
};

using component_type = std::variant<std::monostate, basic_component_type, wire>;

} // namespace sys


