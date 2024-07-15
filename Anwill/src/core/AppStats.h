#pragma once

namespace Anwill {

    struct AppStats
    {
    public:
        inline static long double GetAppUpdatesPerSecond()
        {
            return 1 / layerStackAverageUpdateDuration.GetSeconds();
        }

        inline static long double GetAppUpdateTimeMS()
        {
            return layerStackAverageUpdateDuration.GetMilliseconds();
        }

    private:
        static Timestamp layerStackAverageUpdateDuration;

        friend class App;
    };
}