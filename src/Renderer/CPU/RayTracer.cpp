#include "RayTracer.hpp"
#include "../Texture/CPU_Texture.hpp"
#include <algorithm>
#include <iostream>
#include <memory>
#include <stdexcept>

CPU_RayTracer::CPU_RayTracer(Camera &&camera, int width, int height) {
  // Check if projector is perspective
  if (auto *proj = dynamic_cast<Perspective *>(camera.m_proj.get())) {
    m_camera = std::move(camera);

    // Calculate other stuff
    OnFrameBufferSize(width, height);

  } else {
    throw std::runtime_error("Only support perspective projection for now");
  }
}

void CPU_RayTracer::SetupObjects(
    const std::vector<std::shared_ptr<I_Object3D>> &objects) {
  std::cout << "[Info]: Setting up objects" << std::endl;

  for (auto obj : objects) {
    if (std::shared_ptr<I_RTObject> ptr =
            std::dynamic_pointer_cast<I_RTObject>(obj)) {
      obj->Finalize();
      m_objs.push_back(ptr);
    }
  }
}

std::unique_ptr<I_Texture2D> CPU_RayTracer::Render() {
  Perspective *proj = dynamic_cast<Perspective *>(m_camera.m_proj.get());
  size_t size = m_width * m_height;
  std::vector<float> rawData(size * 3, 0.0f);
  std::vector<std::byte> data(size * 3, std::byte{0});

  if (proj == nullptr) {
    throw std::runtime_error("Only support perspective projection for now");
  }

  const auto C2W = m_camera.m_view->GetC2WMatrix();

#pragma omp parallel for
  for (int y = 0; y < m_height; y++) {
    for (int x = 0; x < m_width; x++) {
      glm::vec4 point_c, point_w;
      point_c = {m_left_c + x * m_du, m_bottom_c + y * m_dv, -proj->m_f, 1.0f};
      point_w = C2W * point_c;

      Ray3D ray = {
          .m_origin{m_camera.m_view->m_pos, 1.0f},
          .m_dir{glm::normalize(glm::vec3(point_w) - m_camera.m_view->m_pos),
                 0.0f}};

      glm::vec3 color = TraceRay(ray, 0, nullptr);

      int pixelIndex = (y * m_width + x) * 3;
      rawData[pixelIndex + 0] = color[0];
      rawData[pixelIndex + 1] = color[1];
      rawData[pixelIndex + 2] = color[2];
    }
  }

  // Normalize the image
  float m = 0.0f, M = 1.0f;

#pragma omp parallel for
  for (int i = 0; i < 3 * size; i++) {
    m = std::min(m, rawData[i]);
    M = std::max(M, rawData[i]);
  }

  std::cout << "[Info]: (m, M) = (" << m << ", " << M << ")" << std::endl;

#pragma omp parallel for
  for (int i = 0; i < 3 * size; i++)
    rawData[i] = 255.0f * (rawData[i] - m) / (M - m);

// Convert from float to byte
#pragma omp parallel for
  for (int i = 0; i < 3 * size; i++) {
    data[i] = static_cast<std::byte>(int(rawData[i]));
  }

  auto ret = std::make_unique<CPU_Texture2D>();
  ret->WriteData(data, {m_width, m_height}, 3);
  return ret;
}

glm::vec3 CPU_RayTracer::TraceRay(const Ray3D &ray, int depth,
                                  std::shared_ptr<I_Object3D> Os) {
  for (auto obj : m_objs) {
    auto in = obj->Intersect(ray);
    if (in.obj)
      // return glm::vec3(in.lambda);
      return (in.n + glm::vec3(1.0f)) / 2.0f;
  }

  return {0.0f, 0.0f, 0.0f};
}

void CPU_RayTracer::OnFrameBufferSize(int width, int height) {
  m_width = width;
  m_height = height;

  /***** Calculate pixel size and some stuff *****/
  float width_pix = width;
  float height_pix = height;

  // Left and bottom in camera coordinates
  m_left_c = -1.0f;
  m_bottom_c = -height_pix / width_pix;

  // Image plane size in camera coordinates
  m_sx = 2.0f;
  m_sy = -2.0f * m_bottom_c;

  // Pixel size in camera coordinates
  m_du = m_sx / width_pix;
  m_dv = m_sy / height_pix;

  // Printing stuff
  std::cout << "[DBG]: " << "m_du: " << m_du << std::endl;
  std::cout << "[DBG]: " << "m_dv: " << m_dv << std::endl;
  std::cout << "[DBG]: " << "m_left_c: " << m_left_c << std::endl;
  std::cout << "[DBG]: " << "m_bottom_c: " << m_bottom_c << std::endl;
}
