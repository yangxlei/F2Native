#include "graphics/animate/GeomAnimate.h"
#include "graphics/canvas/Container.h"
#include "graphics/canvas/Coord.h"
#include "graphics/geom/attr/AttrBase.h"
#include "graphics/shape/Group.h"
#include <map>
#include <set>
#include <utils/Tracer.h>

#ifndef GRAPHICS_GEOM_GEOM_H
#define GRAPHICS_GEOM_GEOM_H

#define GROUP_ATTRS                                                                                                            \
    vector<xg::attr::AttrType> { xg::attr::AttrType::Color, xg::attr::AttrType::Size, xg::attr::AttrType::Shape }
#define COLORS                                                                                                                 \
    std::vector<string> { "#1890FF", "#2FC25B", "#FACC14", "#223273", "#8543E0", "#13C2C2", "#3436C7", "#F04864" }
#define FIELD_ORIGIN "_origin"
#define FIELD_ORIGIN_Y "_originY"

using namespace xg::scale;
using namespace xg::attr;
using namespace xg::canvas;
using namespace xg::shape;
using namespace xg::canvas::coord;
using namespace std;

namespace xg {

class XChart;

namespace geom {

class AbstractGeom {
    friend animate::GeomAnimate;

  public:
    virtual ~AbstractGeom();
    AbstractGeom(const AbstractGeom &) = delete;

    AbstractGeom &operator=(const AbstractGeom &) = delete;

    AbstractGeom &Position(const string &field);
    AbstractGeom &Color(const string &field, const vector<string> &colors);
    AbstractGeom &Color(const string &color);
    AbstractGeom &Size(const string &field, const vector<float> &sizes);
    AbstractGeom &Size(const float size);
    AbstractGeom &Shape(const string &field, const vector<string> &shapes);
    AbstractGeom &Shape(const string &shape);
    AbstractGeom &Adjust(const string &adjust);
    AbstractGeom &Style(const std::string &json = "{}");

    const unique_ptr<attr::AttrBase> &GetColor();

    // create scale & attrs & processData
    void Init(XChart *chart);

    void Paint(XChart *chart);

    const std::string &GetXScaleField();
    const std::string &GetYScaleField();

    std::string GetType() const { return this->type_; }

    bool ContainsAttr(attr::AttrType type);

    nlohmann::json GetSnapRecords(XChart *chart, util::Point point);

    const std::unique_ptr<AttrBase> &GetAttr(AttrType type) { return attrs_[type]; }

    virtual void Clear();

    const nlohmann::json &GetDataArray() { return dataArray_; }

    virtual void SetAttrs(const std::string &_attrs) noexcept;

  protected:
    AbstractGeom(Group *_container, utils::Tracer *tracer) : container_(_container), tracker_(tracer){};
    // 数据分组
    void InitAttributes(XChart &chart);
    void ProcessData(XChart &chart);
    nlohmann::json GroupData(XChart &chart);
    const set<string> GetGroupFieldNames(XChart &chart);
    // void GetAttrValues(XChart &chart, const AttrBase &attr, AttrCfg &acfg, DrawCfg &dcfg);

    virtual void Mapping(XChart &chart, nlohmann::json &data, std::size_t start, std::size_t end);

    virtual void BeforeMapping(XChart &chart, nlohmann::json &dataArray) {}

    virtual void Draw(XChart &chart, const nlohmann::json &groupData, std::size_t start, std::size_t end) const;

    void updateStackRange(XChart &chart);

    virtual double GetYMinValue(XChart &chart);

  protected:
    /*各种属性*/
    bool generatePoints_ = false;
    bool sortable_ = false;
    bool visiable_ = true;
    bool connectNulls_ = false;
    bool ignoreEmptyGroup_ = false;
    string type_ = "";
    string shapeType_ = "";
    nlohmann::json styleConfig_ = {{"startOnZero", true}};

    nlohmann::json dataArray_;
    map<AttrType, unique_ptr<AttrBase>> attrs_{};

    Group *container_ = nullptr;
    utils::Tracer *tracker_ = nullptr;
};
} // namespace geom
} // namespace xg

#endif // GRAPHICS_GEOM_GEOM_H
