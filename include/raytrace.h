/*
 *  Ray Tracing Functions
 */
#ifndef RAYTRACE_H
#define RAYTRACE_H

#define RAYTRACE_MISS   3e+30


/*
 *  raytracePlane
 *
 *  Find the intersection of a ray and a plane.
 *  A negative distance indicates the intersection is behind the ray
 *  origin. This will only occur if the ray origin is on the "back"
 *  side of the plane.
 *
 *  Center  A point on the plane.
 *  Normal  A vector normal to the plane, indicating the "front" side. Must be normalized.
 *
 *  Origin  The ray origin (starting point).
 *  Ray     The ray direction vector. Must be normalized.
 *
 *  Intersection    The closest plane intersection point along the ray.
 *  distance        The distance from the ray Origin to the plane Intersection point.
 *
 *  hit     0 if there is no intersection, 1 otherwise. If Ray
 *          is parallel to the plane, there is no intersection.
 */
void raytracePlane
(
    point   Center;
    vector  Normal;

    point   Origin;
    vector  Ray;
    
    output point    Intersection;
    output float    distance, hit;
) {
    float   nr, no;
    filterregion    fr;

    nr = -Normal.Ray;
    no = Normal.(Origin-Center);
    distance = no / nr;

    // Anything that would intersect really far away is no good.
    // Try and filter that out.
    fr->calculate1d(nr);
    hit = filterstep (no/1e10 + 0.5*fr->maxsize(), nr, nr+fr->maxsize());

#if 0
    // When distance is changing rapidly, the results will be
    // increasingly useless and poorly filtered.
    fr->calculate1d(abs(distance));
    hit = min (hit, 1-filterstep (50, fr->maxsize()));
#endif

    // Remove nans.
    if (hit == hit) {} else {
        hit = 0;
        distance = sign(nr)*RAYTRACE_MISS;
    }
    if (distance == distance) {} else {
        hit = 0;
        distance = sign(nr)*RAYTRACE_MISS;
    }

    distance = mix (RAYTRACE_MISS, distance, clamp(hit,0,1));

    Intersection = Origin + Ray*distance;
}


/*
 *  The box is defined such that the sides
 *  are located at the +/-1 locations of each axis.
 */
void raytraceBox
(
    point   Origin;
    vector  Ray;

    output point    Intersection;
    output float    distance, hit;
) {
    uniform float   i;
    uniform point   cent[] = {(1,0,0), (-1,0,0), (0,1,0), (0,-1,0), (0,0,1), (0,0,-1)};
    uniform vector  norm[] = {(-1,0,0), (1,0,0), (0,-1,0), (0,1,0), (0,0,-1), (0,0,1)};

    point   Q;
    float   d, h;

    hit = 0;
    Intersection = point 0;
    distance = RAYTRACE_MISS;

    if (abs(Origin[0]) < 1 && abs(Origin[1]) < 1 && abs(Origin[2]) < 1) {
        for (i=0; i<6; i+=1) {
            raytracePlane (cent[i], norm[i], Origin, Ray, Q, d, h);
            if (d < distance) {
                distance = d;
                hit = h;
                Intersection = Q;
            }
        }
    }
}


/*
 *  raytraceEllipsoid
 *
 *  Find the intersection of a ray and an ellipsoid. Only intersections
 *  along the ray direction will be found. Intersections behind the ray
 *  origin or at the ray origin are misses.
 *
 *  Notes:
 *  The distance epsilon should really be dependent on the ellipsoid
 *  size as defined by Ellipse rather than being a fixed constant.
 *  
 *  Center      The origin of the ellipsoid.
 *  Ellipse     Given an ellipsoid defined by (can ignore the center)
 *
 *      x²   y²   z²
 *      -- + -- + -- - 1 = 0
 *      a²   b²   c²
 *
 *  then Ellipse equals
 *
 *      (1/a², 1/b², 1/c²)
 *
 *
 *  Origin  The ray origin (starting point).
 *  Ray     The ray direction vector. Must be normalized.
 *          This must be in the same coordinate space as
 *          the ellipsoid is defined in: the ellipsoid is
 *          oriented with the canonical axes (from the
 *          definition of Ellipse above).
 *
 *  Intersection    The closest ellipsoid intersection point along the ray.
 *  SurfaceN        The ellipsoid surface normal (not normalized).
 *                  Points outward, away from the ellipoid center.
 *  distance        The distance from the ray Origin to the ellipsoid Intersection point.
 *
 *  hit     0 if there is no intersection, 1 otherwise.
 *
 */
void raytraceEllipsoid
(
    point   Center;
    vector  Ellipse;

    point   Origin;
    vector  Ray;
    
    output point    Intersection;
    output normal   SurfaceN;
    output float    distance, hit;
) {
    float   a, b, c, disc, t0, t1;
    vector  O;

    O = Origin - Center;

    a = (Ray*Ray).Ellipse;
    b = 2*((O*Ray).Ellipse);
    c = (O*O).Ellipse - 1;
    disc = b*b - 4*a*c;
    
    hit = filterstep (0, disc);

    disc = sqrt(abs(disc));
    a *= 2;
    t0 = (-disc - b) / a;
    t1 = (disc - b) / a;

    distance = min (t0, t1);
    if (distance < .00001) distance = max (t0, t1);

    Intersection = point O + distance*Ray;
    SurfaceN = normal 2*Ellipse * Intersection;
    Intersection += vector Center;
    hit *= filterstep (0, distance);
}


/*
 *  raytraceEllipsoidLine
 *
 *  Find both intersections of a line and an ellipsoid.
 *
 *  Center  The origin of the ellipsoid.
 *  Ellipse Given an ellipsoid defined by (can ignore the center)
 *
 *      x²   y²   z²
 *      -- + -- + -- - 1 = 0
 *      a²   b²   c²
 *
 *  then Ellipse equals
 *
 *      (1/a², 1/b², 1/c²)
 *
 *
 *  Origin  A point on the line.
 *  Ray     A direction vector indicating the orientation of the
 *          line. Must be normalized.
 *          This must be in the same coordinate space as
 *          the ellipsoid is defined in: the ellipsoid is
 *          oriented with the canonical axes (from the
 *          definition of Ellipse above).
 *
 *  Intersection[2] The ellipsoid/line intersection points.
 *  SurfaceN[2]     The ellipsoid surface normals (not normalized).
 *                  Points outward, away from the ellipsoid center.
 *  distance[2]     The distances from the line Origin to the ellipsoid Intersection points.
 *
 *  hit[2]  0 if there is no intersection, 1 otherwise.
 *
 */
void raytraceEllipsoidLine
(
    point   Center;
    vector  Ellipse;

    point   Origin;
    vector  Ray;
    
    output point    Intersection[2];
    output normal   SurfaceN[2];
    output float    distance[2], hit[2];
) {
    float   a, b, c, disc, t0, t1;
    vector  O;

    O = Origin - Center;

    a = (Ray*Ray).Ellipse;
    b = 2*((O*Ray).Ellipse);
    c = (O*O).Ellipse - 1;
    disc = b*b - 4*a*c;
    
    hit[0] = hit[1] = filterstep (0, disc);
    
    disc = sqrt(abs(disc));
    a *= 2;
    distance[0] = (-disc - b) / a;
    distance[1] = (disc - b) / a;

    Intersection[0] = point O + distance[0]*Ray;
    SurfaceN[0] = normal 2*Ellipse * Intersection[0];
    Intersection[0] += vector Center;

    Intersection[1] = point O + distance[1]*Ray;
    SurfaceN[1] = normal 2*Ellipse * Intersection[1];
    Intersection[1] += vector Center;
}


/*
 *  raytraceSphere
 *
 *  Find the intersection of a ray and a sphere. Only intersections
 *  along the ray direction will be found. Intersections behind the ray
 *  origin or at the ray origin are misses as are rays tangent to the
 *  sphere.
 *
 *  Center  The origin of the sphere.
 *  Radius  The radius of the sphere.
 *
 *  Origin  The ray origin (starting point).
 *  Ray     The ray direction vector. Must be normalized.
 *
 *  Intersection    The closest sphere intersection point along the ray.
 *  distance        The distance from the ray Origin to the sphere Intersection point.
 *
 *  hit     0 if there is no intersection, 1 otherwise. If Ray
 *          is parallel to the plane, there is no intersection.
 *
 */
void raytraceSphere
(
    point   Center;
    float   Radius;

    point   Origin;
    vector  Ray;
    
    output point    Intersection;
    output float    distance, hit;
) {
    varying float   rr;
    varying normal  surfaceN;

    rr = Radius*Radius;

    raytraceEllipsoid (Center, vector(1,1,1)/vector(rr,rr,rr), Origin, Ray, Intersection, surfaceN, distance, hit);
}

#endif
