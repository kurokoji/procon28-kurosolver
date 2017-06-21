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
using Point = bg::model::d2::point_xy<long double>;
using Polygon = bg::model::polygon<Point>;
using Segment = bg::model::segment<Point>;

std::istream& operator>>(std::istream& is, Polygon& polygon);
std::ostream& operator<<(std::ostream& os, Polygon& Polygon);

} // namespace procon28

#endif
