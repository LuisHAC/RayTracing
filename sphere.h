#pragma once

#include "hittable.h"
#include "vec3.h"

class sphere : public hittable {
public:
    sphere(point3 _center, double _radius, shared_ptr<material> _material)
        : center(_center), radius(_radius), mat(_material) {}

    /// <summary>
    /// Funciton to determine a ray hit on our sphere.
    /// </summary>
    /// <param name="r">Ray used to calculate hit.</param>
    /// <param name="ray_tmin">Interval of hits: Minimum.</param>
    /// <param name="ray_tmax">Interval of hits: Maximum.</param>
    /// <param name="rec">Record of the hit, if there is any.</param>
    /// <returns>Returns true on ray hit or false if it's outside of boundaries.</returns>
    bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
        vec3 oc = r.origin() - center;
        auto a = r.direction().length_squared();
        auto half_b = dot(oc, r.direction());
        auto c = oc.length_squared() - radius * radius;

        auto discriminant = half_b * half_b - a * c;
        if (discriminant < 0) return false;
        auto sqrtd = sqrt(discriminant);

        // Find the nearest root that lies in the acceptable range.
        auto root = (-half_b - sqrtd) / a;
        if (!ray_t.surrounds(root)) {
            root = (-half_b + sqrtd) / a;
            if (!ray_t.surrounds(root))
                return false;
        }

        rec.t = root;
        rec.p = r.at(rec.t);
        vec3 outward_normal = (rec.p - center) / radius;
        rec.set_face_normal(r, outward_normal);
        rec.mat = mat;

        return true;
    }

private:
    point3 center;
    double radius;
    shared_ptr<material> mat;

};