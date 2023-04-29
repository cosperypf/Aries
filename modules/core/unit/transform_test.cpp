#include "juglans/core/transform.hpp"

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_all.hpp>

using namespace Catch;
using namespace juglans;

TEST_CASE("Vector3 operations are correct", "[transform]") {
  Vector3 a(1.f, 2.f, 3.f);
  Vector3 b(2.5f, -1.4f, 0.6f);

  SECTION("Constructor and assignment") {
    REQUIRE(a.x() == Approx(1.f));
    REQUIRE(a.y() == Approx(2.f));
    REQUIRE(a.z() == Approx(3.f));

    Vector3 c = b;
    REQUIRE(b.x() == c.x());
    REQUIRE(b.y() == c.y());
    REQUIRE(b.z() == c.z());
  }

  SECTION("+ and -") {
    Vector3 p = a + b;

    REQUIRE(a.x() + b.x() == Approx(p.x()));
    REQUIRE(a.y() + b.y() == Approx(p.y()));
    REQUIRE(a.z() + b.z() == Approx(p.z()));

    Vector3 m = a - b;
    REQUIRE(a.x() - b.x() == Approx(m.x()));
    REQUIRE(a.y() - b.y() == Approx(m.y()));
    REQUIRE(a.z() - b.z() == Approx(m.z()));
  }

  SECTION("* and /") {
    Vector3 m = a * 0.5f;

    REQUIRE(m.x() * 2.f == Approx(a.x()));
    REQUIRE(m.y() * 2.f == Approx(a.y()));
    REQUIRE(m.z() * 2.f == Approx(a.z()));

    Vector3 d = a / 1.5f;
    REQUIRE(d.x() * 1.5f == Approx(a.x()));
    REQUIRE(d.y() * 1.5f == Approx(a.y()));
    REQUIRE(d.z() * 1.5f == Approx(a.z()));
  }

  SECTION("Inverse") {
    Vector3 i = -a;
    REQUIRE(a.x() == Approx(-i.x()));
    REQUIRE(a.y() == Approx(-i.y()));
    REQUIRE(a.z() == Approx(-i.z()));
  }

  SECTION("Dot and cross") {
    number_t d1 = a.dot(b);
    number_t d2 = b.dot(a);
    REQUIRE(d1 == Approx(d2));
    REQUIRE(d1 == Approx(a.x() * b.x() + a.y() * b.y() + a.z() * b.z()));

    Vector3 c = a.cross(b);
    // NOTE: Approx(0) has no effect, see
    // https://github.com/catchorg/Catch2/issues/1444#issuecomment-439725621
    REQUIRE_THAT(a.dot(c), Matchers::WithinAbs(0.f, 1e-6f));
    REQUIRE_THAT(b.dot(c), Matchers::WithinAbs(0.f, 1e-6f));
  }
}

TEST_CASE("Quaternion operations are correct", "[transform]") {
  Quaternion a(1.5f, 1.f, -2.f, 0.5f);
  Quaternion b(1.5f, 2.4f, -0.7f, -0.9f);
  Vector3 v(2.5f, -1.4f, 0.6f);

  SECTION("Constructor and assignment") {
    REQUIRE(a.w() == Approx(1.5f));
    REQUIRE(a.x() == Approx(1.f));
    REQUIRE(a.y() == Approx(-2.f));
    REQUIRE(a.z() == Approx(0.5f));

    Quaternion c = b;
    REQUIRE(b.w() == c.w());
    REQUIRE(b.x() == c.x());
    REQUIRE(b.y() == c.y());
    REQUIRE(b.z() == c.z());
  }

  SECTION("Dot") {
    number_t d1 = a.dot(b);
    number_t d2 = b.dot(a);
    REQUIRE(d1 == Approx(d2));
    REQUIRE(d1 == Approx(a.w() * a.w() + a.x() * b.x() + a.y() * b.y() + a.z() * b.z()));
  }

  SECTION("Normalize") {
    Quaternion c = a.normalized();
    number_t scale = a.w() / c.w();
    REQUIRE(c.x() * scale == Approx(a.x()));
    REQUIRE(c.y() * scale == Approx(a.y()));
    REQUIRE(c.z() * scale == Approx(a.z()));
    REQUIRE(c.dot(c) == Approx(1.f));

    a.normalize();
    REQUIRE(a.w() == Approx(c.w()));
    REQUIRE(a.x() == Approx(c.x()));
    REQUIRE(a.y() == Approx(c.y()));
    REQUIRE(a.z() == Approx(c.z()));
  }

  a.normalize();

  SECTION("Inverse") {
    Quaternion i = a.inverse();
    REQUIRE(a.w() == Approx(i.w()));
    REQUIRE(a.x() == Approx(-i.x()));
    REQUIRE(a.y() == Approx(-i.y()));
    REQUIRE(a.z() == Approx(-i.z()));
  }

  SECTION("*") {
    Quaternion i1 = a.inverse() * a;
    REQUIRE(i1.w() == Approx(1.f));
    REQUIRE_THAT(i1.x(), Matchers::WithinAbs(0.f, 1e-6f));
    REQUIRE_THAT(i1.y(), Matchers::WithinAbs(0.f, 1e-6f));
    REQUIRE_THAT(i1.z(), Matchers::WithinAbs(0.f, 1e-6f));

    Quaternion i2 = a * a.inverse();
    REQUIRE(i2.w() == Approx(1.f));
    REQUIRE_THAT(i2.x(), Matchers::WithinAbs(0.f, 1e-6f));
    REQUIRE_THAT(i2.y(), Matchers::WithinAbs(0.f, 1e-6f));
    REQUIRE_THAT(i2.z(), Matchers::WithinAbs(0.f, 1e-6f));

    Vector3 v2 = a * v;
    Quaternion q3 = a * Quaternion(0, v.x(), v.y(), v.z()) * a.inverse();
    REQUIRE(v2.x() == Approx(q3.x()));
    REQUIRE(v2.y() == Approx(q3.y()));
    REQUIRE(v2.z() == Approx(q3.z()));
  }
}

TEST_CASE("Transform operations are correct", "[transform]") {
  Quaternion q(1.5f, 1.f, -2.f, 0.5f);
  Vector3 v(2.5f, -1.4f, 0.6f);
  Transform t(v, q);

  SECTION("Constructor and assignment") {
    REQUIRE(t.position.x() == Approx(v.x()));
    REQUIRE(t.position.y() == Approx(v.y()));
    REQUIRE(t.position.z() == Approx(v.z()));
    REQUIRE(t.rotation.w() == Approx(q.w()));
    REQUIRE(t.rotation.x() == Approx(q.x()));
    REQUIRE(t.rotation.y() == Approx(q.y()));
    REQUIRE(t.rotation.z() == Approx(q.z()));

    Transform c = t;
    REQUIRE(c.position.x() == Approx(t.position.x()));
    REQUIRE(c.position.y() == Approx(t.position.y()));
    REQUIRE(c.position.z() == Approx(t.position.z()));
    REQUIRE(c.rotation.w() == Approx(t.rotation.w()));
    REQUIRE(c.rotation.x() == Approx(t.rotation.x()));
    REQUIRE(c.rotation.y() == Approx(t.rotation.y()));
    REQUIRE(c.rotation.z() == Approx(t.rotation.z()));
  }

  SECTION("Inverse") {
    t.rotation.normalize();
    Transform i = t.inverse();
    Transform i1 = i * t;
    REQUIRE(i1.rotation.w() == Approx(1.f));
    REQUIRE_THAT(i1.rotation.x(), Matchers::WithinAbs(0.f, 1e-6f));
    REQUIRE_THAT(i1.rotation.y(), Matchers::WithinAbs(0.f, 1e-6f));
    REQUIRE_THAT(i1.rotation.z(), Matchers::WithinAbs(0.f, 1e-6f));
    REQUIRE_THAT(i1.position.x(), Matchers::WithinAbs(0.f, 1e-6f));
    REQUIRE_THAT(i1.position.y(), Matchers::WithinAbs(0.f, 1e-6f));
    REQUIRE_THAT(i1.position.z(), Matchers::WithinAbs(0.f, 1e-6f));

    Transform i2 = t * i;
    REQUIRE(i2.rotation.w() == Approx(1.f));
    REQUIRE_THAT(i2.rotation.x(), Matchers::WithinAbs(0.f, 1e-6f));
    REQUIRE_THAT(i2.rotation.y(), Matchers::WithinAbs(0.f, 1e-6f));
    REQUIRE_THAT(i2.rotation.z(), Matchers::WithinAbs(0.f, 1e-6f));
    REQUIRE_THAT(i2.position.x(), Matchers::WithinAbs(0.f, 1e-5f));
    REQUIRE_THAT(i2.position.y(), Matchers::WithinAbs(0.f, 1e-5f));
    REQUIRE_THAT(i2.position.z(), Matchers::WithinAbs(0.f, 1e-5f));
  }
}
