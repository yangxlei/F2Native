#include "graphics/canvas/Coord.h"
#include "graphics/util/Matrix.h"
#include "graphics/util/Point.h"
#include "graphics/util/Vector2d.h"
#include <math.h>

#ifndef XG_GRAPHICS_COORD_POLAR_H
#define XG_GRAPHICS_COORD_POLAR_H

namespace xg {
namespace canvas {
namespace coord {

// 级坐标
class Polar : public AbstractCoord {
  public:
    // @param start -- 左下角坐标
    // @param end -- 右上角坐标
    // @param transposed -- 坐标系是否翻转
    Polar(util::Point start, util::Point end, bool transposed = false) : AbstractCoord() {
        transposed_ = transposed;
        Reset(start, end);
    }

    // @param point -- 度量坐标值[0~1]
    // @return  实际坐标值
    util::Point ConvertPoint(util::Point point) const override {

        double angle = xAxis_.x + (xAxis_.y - xAxis_.x) * (transposed_ ? point.y : point.x);
        double radius = yAxis_.x + (yAxis_.y - yAxis_.x) * (transposed_ ? point.x : point.y);

        double x = center_.x + std::cos(angle) * radius;
        double y = center_.y + std::sin(angle) * radius;

        util::Vector2D v;
        util::Vector2DUtil::Reset(&v, x, y);
        util::Vector2DUtil::TransformMat2D(&v, v, this->matrix_);

        return util::Point{v[0], v[1]};
    }

    // @param point -- 实际坐标值
    // @return 度量坐标值
    util::Point InvertPoint(util::Point point) const override {
        util::Matrix m;
        util::MatrixUtil::Rotate(&m, this->matrix_, xAxis_.x);
        util::Vector2D startV;
        util::Vector2D v = {1, 0};
        util::Vector2DUtil::TransformMat2D(&startV, v, m);
        util::Vector2D pointV = {point.x - center_.x, point.y - center_.y};
        if(Vector2DUtil::Zero(pointV)) {
            return std::move(util::Point(0, 0));
        }
        double theta = Vector2DUtil::AngleTo(startV, pointV, xAxis_.y < xAxis_.x);
        if(abs(theta - M_PI * 2) < 0.001) {
            theta = 0;
        }
        double l = Vector2DUtil::Length(pointV);
        double percentX = theta / (xAxis_.y - xAxis_.x);
        percentX = xAxis_.y - xAxis_.x > 0 ? percentX : -percentX;
        double percentY = (l - yAxis_.y) / (yAxis_.y - yAxis_.x);
        if(this->transposed_) {
            return std::move(util::Point(percentY, percentX));
        } else {
            return std::move(util::Point(percentX, percentY));
        }
    }

    virtual void Reset(util::Point start, util::Point end) noexcept override {
        util::MatrixUtil::Reset(&this->matrix_);
        double width = abs(end.x - start.x);
        double height = abs(end.y - start.y);
        if(fabs(startAngle_ + M_PI) <= XG_EPS && fabs(endAngle_) <= XG_EPS) {
            circleRadius_ = fmin(width / 2, height);
            center_.Reset((start.x + end.x) / 2, start.y);
        } else {
            circleRadius_ = fmin(width, height) / 2;
            center_.Reset(start.x + circleRadius_, end.y + circleRadius_);
        }
        xAxis_.Reset(startAngle_, endAngle_);
        yAxis_.Reset(0, circleRadius_);
    }

    virtual CoordType GetType() const noexcept override { return CoordType::Polar; }

    float GetWidth() const noexcept override { return xAxis_.y - xAxis_.x; }

    double GetRadius() const noexcept override { return circleRadius_; }

    util::Point GetXAxis() noexcept override { return xAxis_; }

    util::Point GetYAxis() noexcept override { return yAxis_; }

    bool IsContains(double x, double y) noexcept override {
        return (x >= xAxis_.x && x <= xAxis_.y && y >= yAxis_.x && y <= yAxis_.y);
    }

  private:
    util::Point xAxis_; // x 轴的坐标 [x 起点， x 终点]
    util::Point yAxis_; // y 轴的坐标 [y 起点， y 终点]
    util::Matrix matrix_;

  public:
    double startAngle_ = -M_PI / 2;
    double endAngle_ = M_PI * 3 / 2;
    double circleRadius_ = 0;
};
} // namespace coord
} // namespace canvas
} // namespace xg

#endif /* XG_GRAPHICS_COORD_POLAR_H */
