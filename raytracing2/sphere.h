#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"
#include "aabb.h"

class sphere : public hittable {
public:
    sphere(point3 _center, double _radius, shared_ptr<material> _material): center1(_center), radius(_radius), mat(_material), is_moving(false){
        auto rvec = vec3(radius, radius, radius);
        bbox = aabb(center1 - rvec, center1 + rvec);
    }

    aabb bounding_box() const override {return bbox;}

    sphere(point3 _center1, point3 _center2, double _radius, shared_ptr<material> _material): center1(_center1), center2(_center2), radius(_radius), mat(_material), is_moving(true){
        auto rvec = vec3(radius, radius, radius);
        aabb box1(_center1 - rvec, _center1 + rvec);
        aabb box2(_center2 - rvec, _center2 + rvec);
        bbox = aabb(box1, box2);
        center_vec = _center2 - center1;
    }

    bool hit(const ray& r, interval ray_t, hit_record& rec) const override{
        point3 center = is_moving ? sphere_center(r.time()) : center1;
        vec3 oc = r.origin() - center;
        auto a = r.direction().length_squared();
        auto half_b = dot(oc, r.direction());
        auto c = oc.length_squared() - radius*radius;

        auto discriminant = half_b*half_b - a*c;
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
        rec.normal = (rec.p - center) / radius;
        vec3 outword_normal = (rec.p - center) / radius;
        rec.set_face_normal(r, outword_normal);
        rec.mat = mat;

        return true;
    }



private:
    point3 center1, center2;
    double radius;
    shared_ptr<material> mat;
    bool is_moving;
    vec3 center_vec;
    aabb bbox;

    point3 sphere_center(double time) const {
        return center1 + time * center_vec;
    }
};




#endif