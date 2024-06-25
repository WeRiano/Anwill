#pragma once

namespace Anwill {

    enum class MouseButtonCode : unsigned int {

        Button1 = 0,
        Button2 = 1,
        Button3 = 2,
        Button4 = 3,
        Button5 = 4,
        Button6 = 5,
        Button7 = 6,
        Button8 = 7,
        ButtonLast = Button8,
        ButtonLeft = Button1,
        ButtonRight = Button2,
        ButtonMiddle = Button3
    };

    enum class MouseScrollCode : int {
        Right = -1,
        Left = 1,
        Down = 0,
        Up = 2,
    };
}