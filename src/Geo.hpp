#ifndef PROCON_28_GEO_HPP
#define PROCON_28_GEO_HPP

#include <iostream>
#include <vector>

#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/geometries/segment.hpp>
#include <boost/geometry/geometry.hpp>

namespace bg = boost::geometry;

namespace procon28 {
using Point_Type = long double;  // x, y座標に使用する型
using Point = bg::model::d2::point_xy<Point_Type>;  // 座標
using Polygon = bg::model::polygon<Point>;          // 図形
using Segment = bg::model::segment<Point>;          // 線分
using Ring = typename Polygon::ring_type;

std::istream& operator>>(std::istream&, Polygon&);
std::ostream& operator<<(std::ostream&, Polygon&);

Point get_point(const Ring&, int);
Point get_point(const Polygon&, int);
Segment get_segment(const Ring&, int, int);
Segment get_segment(const Polygon&, int, int);
Segment get_segment(const Ring&, int);
Segment get_segment(const Polygon&, int);
}  // namespace procon28

#endif
