#pragma once


struct GLFWwindow;

class Event {
    public:
            GLFWwindow*     window      = nullptr;
            int             key         = 0;
            int             scancode    = 0;
            int             action      = 0;
            int             mods        = 0;
            double          scrollX     = 0.;
            double          scrollY     = 0.;
};

