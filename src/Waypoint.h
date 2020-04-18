#ifndef WAYPOINT_H
#define WAYPOINT_H

#include <Godot.hpp>
#include <Object.hpp>
#include <Spatial.hpp>
#include <Array.hpp>
#include <tuple>
#include "AIEnemy.h"
#include <Area.hpp>

namespace godot {

    class Waypoint : public Spatial {
        GODOT_CLASS(Waypoint, Spatial);

    private:
        Array connectedWaypoints;
        bool taken;
        bool visible;

    public:
        Vector3 pos;

        static void _register_methods();

        Waypoint();
        ~Waypoint();

        void _init();
        void _ready();

        float get_dist(Vector3 other); // finds distance between input and self
        //marks this Waypoint as taken
        void take();
        //marks this Waypoint as not taken
        void untake();
        //returns whether this Waypoint is taken
        bool is_taken();
        Array get_connected_waypoints();
        void _on_area_entered(Area* other);
        void _on_area_exited(Area* other);
        void toggle_visibility();

    };

}

#endif