#include "rix/rdf/link.hpp"

namespace rix {
namespace rdf {

Link::Link(const Eigen::Affine3d &origin, const std::string &parent, const std::vector<std::string> &children)
    : origin(origin), parent(parent), children(children) {}

Link::Link(const Link &other) {
    origin = other.origin;
    parent = other.parent;
    children = other.children;
}

Link &Link::operator=(Link other) {
    if (this == &other) {
        return *this;
    }
    origin = other.origin;
    parent = other.parent;
    children = other.children;
    return *this;
}

std::string Link::get_parent() const { return parent; }

std::vector<std::string> Link::get_children() const { return children; }

bool Link::is_root() const { return parent.empty(); }

bool Link::is_end_effector() const { return children.empty(); }

const Eigen::Affine3d &Link::get_origin() const { return origin; }

}  // namespace rdf
}  // namespace rix