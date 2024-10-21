#include "Transform.hpp"

namespace Bored {
namespace ECS {

void to_json(json& j, Transform& transform) {
  j["translate"] = {transform.pos.x, transform.pos.y, transform.pos.z};
  j["rotate"] = {transform.rotation.x, transform.rotation.y, transform.rotation.z};
  j["scale"] = {transform.scale.x, transform.scale.y, transform.scale.z};
}

void from_json(const json& j, Transform& transform) {
    std::vector<float> pos, rotate, scale;
    j.at("translate").get_to<std::vector<float>>(pos);
    j.at("rotate").get_to<std::vector<float>>(rotate);
    j.at("scale").get_to<std::vector<float>>(scale);

    transform.pos = {pos[0], pos[1], pos[2]};
    transform.scale = {scale[0], scale[1], scale[2]};
    transform.rotation = {rotate[0], rotate[1], rotate[2]};
}

}  // namespace ECS
}  // namespace Bored
