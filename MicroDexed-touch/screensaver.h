#ifndef _SCREENSAVER_H
#define _SCREENSAVER_H

uint8_t const qix_num = 50;

typedef struct qix_s
{
    float x0s[qix_num], y0s[qix_num], x1s[qix_num], y1s[qix_num];
    float dx0 = 3.3, dx1 = 4.4, dy0 = 4.8, dy1 = 1.3;
} qix_t;

#endif

/*
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef Vector_H
#define Vector_H

template <class T>
class Vector2 {
public:
    T x, y;

    Vector2():x(0), y(0) {}
    Vector2(T x, T y): x(x), y(y) {}
    Vector2(const Vector2& v): x(v.x), y(v.y) {}

    Vector2& operator=(const Vector2& v) {
        x = v.x;
        y = v.y;
        return *this;
    }

    bool operator==(Vector2& v) {
        return x == v.x && y == v.y;
    }

    Vector2 operator+(Vector2& v) {
        return Vector2(x + v.x, y + v.y);
    }
    Vector2 operator-(Vector2& v) {
        return Vector2(x - v.x, y - v.y);
    }

    Vector2& operator+=(Vector2& v) {
        x += v.x;
        y += v.y;
        return *this;
    }
    Vector2& operator-=(Vector2& v) {
        x -= v.x;
        y -= v.y;
        return *this;
    }

    Vector2& operator*=(double s) {
        x *= s;
        y *= s;
        return *this;
    }
    
    Vector2& operator/=(double s) {
        x /= s;
        y /= s;
        return *this;
    }

    Vector2& normalize() {
        if (length() == 0) return *this;
        *this *= (1.0 / length());
        return *this;
    }

    float dist(Vector2 v) const {
        Vector2 d(v.x - x, v.y - y);
        return d.length();
    }
    float length() const {
        return sqrt(x * x + y * y);
    }

    float mag() const {
        return length();
    }

    float magSq() {
        return (x * x + y * y);
    }

    void truncate(double length) {
        double angle = atan2f(y, x);
        x = length * cos(angle);
        y = length * sin(angle);
    }

    void limit(float max) {
        if (magSq() > max * max) {
            normalize();
            *this *= max;
        }
    }
};

typedef Vector2<float> PVector;

#endif



#include "ILI9341_t3n.h"
extern ILI9341_t3n display;
extern uint32_t ColorHSV(uint16_t hue, uint8_t sat, uint8_t val);
extern uint8_t screensaver_brightness;

/*
 * Portions of this code are adapted from "Flocking" in "The Nature of Code" by Daniel Shiffman: http://natureofcode.com/
 * Copyright (c) 2014 Daniel Shiffman
 * http://www.shiffman.net
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

 // Flocking
 // Daniel Shiffman <http://www.shiffman.net>
 // The Nature of Code, Spring 2009

 // Boid class
 // Methods for Separation, Cohesion, Alignment added

static const int boidCount = 70;

class Boid {
public:

    PVector location;
    PVector velocity;
    PVector acceleration;

    float maxforce;    // Maximum steering force
    float maxspeed;    // Maximum speed

    float desiredseparation = 14;
    float neighbordist = 30;
    float mass;

    Boid() {}

    Boid(float x, float y) {
        acceleration = PVector(0, 0);
        velocity = PVector(randomf(), randomf());
        location = PVector(x, y);
        maxspeed = 1.5;
        maxforce = 0.22;
    }

    static float randomf() {
        return mapfloat(random(0, 255), 0, 255, -.5, .5);
    }

    static float mapfloat(float x, float in_min, float in_max, float out_min, float out_max) {
        return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
    }

    void run(Boid boids[]) {
        flock(boids);
        update();
        wrapAroundBorders();
        //bounceOffBorders();
    }

    // Method to update location
    void update() {
        // Update velocity
        velocity += acceleration;
        // Limit speed
        velocity.limit(maxspeed);
        location += velocity;
        // Reset acceleration to 0 each cycle
        acceleration *= 0;
    }

    void applyForce(PVector force) {
        // We could add mass here if we want A = F / M
        acceleration += force;
    }

    void repelForce(PVector obstacle, float radius) {
        //Force that drives boid away from obstacle.

        PVector futPos = location + velocity; //Calculate future position for more effective behavior.
        PVector dist = obstacle - futPos;
        float d = dist.mag();

        if (d <= radius) {
            PVector repelVec = location - obstacle;
            repelVec.normalize();
            if (d != 0) { //Don't divide by zero.
                // float scale = 1.0 / d; //The closer to the obstacle, the stronger the force.
                repelVec.normalize();
                repelVec *= (maxforce * 7);
                if (repelVec.mag() < 0) { //Don't let the boids turn around to avoid the obstacle.
                    repelVec.y = 0;
                }
            }
            applyForce(repelVec);
        }
    }

    // We accumulate a new acceleration each time based on three rules
    void flock(Boid boids[]) {
        PVector sep = separate(boids);   // Separation
        PVector ali = align(boids);      // Alignment
        PVector coh = cohesion(boids);   // Cohesion
        // Arbitrarily weight these forces
        sep *= 1.5;
        ali *= 1.0;
        coh *= 1.0;
        // Add the force vectors to acceleration
        applyForce(sep);
        applyForce(ali);
        applyForce(coh);
    }

    // Separation
    // Method checks for nearby boids and steers away
    PVector separate(Boid boids[]) {
        PVector steer = PVector(0, 0);
        int count = 0;
        // For every boid in the system, check if it's too close
        for (int i = 0; i < boidCount; i++) {
            Boid other = boids[i];
            float d = location.dist(other.location);
            // If the distance is greater than 0 and less than an arbitrary amount (0 when you are yourself)
            if ((d > 0) && (d < desiredseparation)) {
                // Calculate vector pointing away from neighbor
                PVector diff = location - other.location;
                diff.normalize();
                diff /= d;        // Weight by distance
                steer += diff;
                count++;            // Keep track of how many
            }
        }
        // Average -- divide by how many
        if (count > 0) {
            steer /= (float)count;
        }

        // As long as the vector is greater than 0
        if (steer.mag() > 0) {
            // Implement Reynolds: Steering = Desired - Velocity
            steer.normalize();
            steer *= maxspeed;
            steer -= velocity;
            steer.limit(maxforce);
        }
        return steer;
    }

    // Alignment
    // For every nearby boid in the system, calculate the average velocity
    PVector align(Boid boids[]) {
        PVector sum = PVector(0, 0);
        int count = 0;
        for (int i = 0; i < boidCount; i++) {
            Boid other = boids[i];
            float d = location.dist(other.location);
            if ((d > 0) && (d < neighbordist)) {
                sum += other.velocity;
                count++;
            }
        }
        if (count > 0) {
            sum /= (float)count;
            sum.normalize();
            sum *= maxspeed;
            PVector steer = sum - velocity;
            steer.limit(maxforce);
            return steer;
        }
        else {
            return PVector(0, 0);
        }
    }

    // Cohesion
    // For the average location (i.e. center) of all nearby boids, calculate steering vector towards that location
    PVector cohesion(Boid boids[]) {
        PVector sum = PVector(0, 0);   // Start with empty vector to accumulate all locations
        int count = 0;
        for (int i = 0; i < boidCount; i++) {
            Boid other = boids[i];
            float d = location.dist(other.location);
            if ((d > 0) && (d < neighbordist)) {
                sum += other.location; // Add location
                count++;
            }
        }
        if (count > 0) {
            sum /= count;
            return seek(sum);  // Steer towards the location
        }
        else {
            return PVector(0, 0);
        }
    }

    // A method that calculates and applies a steering force towards a target
    // STEER = DESIRED MINUS VELOCITY
    PVector seek(PVector target) {
        PVector desired = target - location;  // A vector pointing from the location to the target
        // Normalize desired and scale to maximum speed
        desired.normalize();
        desired *= maxspeed;
        // Steering = Desired minus Velocity
        PVector steer = desired - velocity;
        steer.limit(maxforce);  // Limit to maximum steering force
        return steer;
    }

    // A method that calculates a steering force towards a target
    // STEER = DESIRED MINUS VELOCITY
    void arrive(PVector target) {
        PVector desired = target - location;  // A vector pointing from the location to the target
        float d = desired.mag();
        // Normalize desired and scale with arbitrary damping within 50 pixels
        desired.normalize();
        if (d < 4) {
            float m = map(d, 0, 50, 0, maxspeed);
            desired *= m;
        }
        else {
            desired *= maxspeed;
        }

        // Steering = Desired minus Velocity
        PVector steer = desired - velocity;
        steer.limit(maxforce);  // Limit to maximum steering force
        applyForce(steer);

    }

    // Wraparound
    void wrapAroundBorders() {
      if (location.x < 0) location.x = DISPLAY_WIDTH - 1;
      if (location.y < 0) location.y = DISPLAY_HEIGHT - 1;
      if (location.x >= DISPLAY_WIDTH) location.x = 0;
      if (location.y >= DISPLAY_HEIGHT) location.y = 0;
    }

    // bool bounceOffBorders() {
    //     bool bounced = false;

    //     if (location.x >= DISPLAY_WIDTH - 1) {
    //         location.x = DISPLAY_WIDTH - 1;
    //         velocity.x *= -1;
    //         bounced = true;
    //     }
    //     else if (location.x < 0) {
    //         location.x = 0;
    //         velocity.x *= -1;
    //         bounced = true;
    //     }

    //     if (location.y >= DISPLAY_HEIGHT - 1) {
    //         location.y = DISPLAY_HEIGHT - 1;
    //         velocity.y *= -1;
    //         bounced = true;
    //     }
    //     else if (location.y < 0) {
    //         location.y = 0;
    //         velocity.y *= -1;
    //         bounced = true;
    //     }
    //     return bounced;
    // }

};



