#pragma once
#include "Common/BlamBaseTypes.h"
#include "API.h"

static void angle_set_degrees(angle *value, double new_value) {
	value->rad = static_cast<float>(new_value / (180.0 / 3.14159265358979323846));
}

class BasicTypesAPI : public LuaAPI {
	inline static std::string vec_to_string(real_vector2d& vec) {
		return stringify("i", vec.i, "j", vec.j);
	}

	inline static std::string vec_to_string(real_vector3d& vec) {
		return stringify("i", vec.i, "j", vec.j, "k", vec.k);
	}

public:
	BasicTypesAPI(sol::state& _lua) : LuaAPI(_lua)
	{
		lua.new_usertype<blam_tag>("blam_tag",
			sol::constructors<blam_tag(uint32_t), blam_tag(const std::string&)>(),
			sol::meta_function::to_string, &blam_tag::as_string,
			"as_string", &blam_tag::as_string,
			"as_int", &blam_tag::as_int
		);

		lua.new_usertype<string_id>("string_id",
			sol::constructors<string_id(), string_id(uint32_t, uint8_t), string_id(uint32_t), string_id(const std::string&)>(),
			sol::meta_function::to_string, &string_id::get_name,
			"get_name", &string_id::get_name,
			"get_packed", &string_id::get_packed,
			"get_id", &string_id::get_id,
			"get_length", &string_id::get_length
		);

		lua.new_usertype<angle>("angle",
			sol::constructors<angle(), angle(float)>(),
			sol::meta_function::to_string, [](angle& angle) -> std::string { return std::to_string(angle.rad); },
			"rad", &angle::rad,
			"degree", sol::property(&angle::as_degree, &angle_set_degrees)
		);

		lua.new_usertype<rect2d>("rectangle_2d",
			sol::constructors<rect2d()>(),
			sol::meta_function::to_string, [](rect2d& rect) -> std::string {
				return stringify("bottom", rect.bottom, "top", rect.top, "left", rect.left, "right", rect.right);
			},
			"bottom", &rect2d::bottom,
			"left", &rect2d::left,
			"right", &rect2d::right,
			"top", &rect2d::top
		);

		lua.new_usertype<point2d>("point_2d",
			sol::constructors<point2d()>(),
			sol::meta_function::to_string, [](point2d& point) -> std::string {
				return stringify("x", point.x, "y", point.y);
			},
			"x", &point2d::x,
			"y", &point2d::y
		);

		lua.new_usertype<real_point2d>("real_point_2d",
			sol::constructors<real_point2d()>(),
			sol::meta_function::to_string, [](real_point2d& point) -> std::string {
				return stringify("x", point.x, "y", point.y);
			}, 
			"x", &real_point2d::x,
			"y", &real_point2d::y
		);

		lua.new_usertype<real_point3d>("real_point_3d",
			sol::constructors<real_point3d()>(),
			sol::meta_function::to_string, [](real_point3d& point) -> std::string {
				return stringify("x", point.x, "y", point.y, "z", point.z);
			},
			"x", &real_point3d::x,
			"y", &real_point3d::y,
			"z", &real_point3d::z
		);

		lua.new_usertype<real_vector2d>("real_vector_2d",
			sol::constructors<real_vector2d()>(),
			sol::meta_function::to_string, [](real_vector2d& vec) -> std::string {
				return vec_to_string(vec);
			},
			"i", &real_vector2d::i,
			"j", &real_vector2d::j
		);

		lua.new_usertype<real_vector3d>("real_vector_3d",
			sol::constructors<real_vector3d()>(),
			sol::meta_function::to_string, [](real_vector3d& vec) -> std::string {
				return vec_to_string(vec);
			},
			"i", & real_vector3d::i,
			"j", & real_vector3d::j,
			"k", & real_vector3d::k
		);

		lua.new_usertype<real_quaternion>("real_quaternion",
			sol::constructors<real_quaternion()>(),
			sol::meta_function::to_string, [](real_quaternion& qaut) -> std::string {
				return stringify("i", qaut.i, "j", qaut.j, "k", qaut.k, "w", qaut.w);
			},
			"i", & real_quaternion::i,
			"j", & real_quaternion::j,
			"k", & real_quaternion::k,
			"w", & real_quaternion::w,
			"norm_square", sol::readonly_property(&real_quaternion::get_square_length),
			"norm", sol::readonly_property([](real_quaternion& quat) -> float { return sqrtf(quat.get_square_length()); })
		);

		lua.new_usertype<real_euler_angles2d>("real_euler_angles_2d",
			sol::constructors<real_euler_angles2d()>(),
			sol::meta_function::to_string, [](real_euler_angles2d& angle_2d) -> std::string {
				return stringify("pitch", angle_2d.pitch.as_rad(), "yaw", angle_2d.yaw.as_rad());
			},
			"pitch", &real_euler_angles2d::pitch,
			"yaw", &real_euler_angles2d::yaw
		);

		lua.new_usertype<real_euler_angles2d>("real_euler_angles_2d",
			sol::constructors<real_euler_angles2d()>(),
			sol::meta_function::to_string, [](real_euler_angles2d& angle_2d) -> std::string {
				return stringify("pitch", angle_2d.pitch.as_rad(), "yaw", angle_2d.yaw.as_rad());
			},
			"pitch", &real_euler_angles2d::pitch,
			"yaw", &real_euler_angles2d::yaw
		);

		lua.new_usertype<real_plane2d>("real_plane_2d",
			sol::constructors<real_plane2d()>(),
			sol::meta_function::to_string, [](real_plane2d& plane) -> std::string {
				return stringify(
					"normal", vec_to_string(plane.normal),
					"distance", plane.distance
				);
			},
			"normal", &real_plane2d::normal,
			"distance", &real_plane2d::distance
		);

		lua.new_usertype<real_plane3d>("real_plane_3d",
			sol::constructors<real_plane3d()>(),
			sol::meta_function::to_string, [](real_plane3d& plane) -> std::string {
				return stringify(
					"normal", vec_to_string(plane.normal),
					"distance", plane.distance
				);
			},
			"normal", &real_plane3d::normal,
			"distance", &real_plane3d::distance
		);

		lua.new_usertype<colour_rgb>("real_rgb_color",
			sol::constructors<colour_rgb(), colour_rgb(float, float, float)>(),
			sol::meta_function::to_string, [](colour_rgb& colour) -> std::string {
				return stringify(
					"red", colour.red,
					"green", colour.green,
					"blue", colour.blue
				);
			},
			"red", &colour_rgb::red,
			"green", &colour_rgb::green,
			"blue", &colour_rgb::blue
		);
		lua.new_usertype<colour_rgba>("real_rgba_color",
			sol::constructors<colour_rgba(), colour_rgba(float, float, float, float)>(),
			sol::meta_function::to_string, [](colour_rgba& colour) -> std::string {
				return stringify(
					"red", colour.red,
					"green", colour.green,
					"blue", colour.blue,
					"alpha", colour.alpha
				);
			},
			"red", &colour_rgba::red,
			"green", &colour_rgba::green,
			"blue", &colour_rgba::blue,
			"alpha", &colour_rgba::alpha
		);
	};
};
