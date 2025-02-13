#include "graphics/shape/Shape.h"

#ifndef XG_GRAPHICS_SHAPE_MARKER_H
#define XG_GRAPHICS_SHAPE_MARKER_H

namespace xg {
namespace shape {

class Marker : public Shape {
  public:
    Marker(const util::Point &center, const float radius, const std::string &fill, const std::string &symbol)
        : radius_(radius), symbol_(symbol) {
        type_ = "marker";
        point_ = center;
        fillStyle_ = util::CanvasFillStrokeStyle(fill);
    }

    BBox CalculateBox(canvas::CanvasContext &context) const override;

  protected:
    void CreatePath(canvas::CanvasContext &context) const override;

  public:
    float radius_;
    std::string symbol_ = "circle";
};

} // namespace shape
} // namespace xg

#endif /* XG_GRAPHICS_SHAPE_MARKER_H */
