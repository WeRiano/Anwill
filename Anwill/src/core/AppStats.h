#pragma once

namespace Anwill {

    struct AppStats
    {
    public:
        inline static void PushUpdateDuration(const Timestamp& updateDuration)
        {
            layerStackUpdateDurations.push_back(updateDuration);
            if(layerStackUpdateDurations.size() > 100)
            {
                layerStackUpdateDurations.pop_front();
            }

            Timestamp average = Timestamp(0);
            for(int i = 0; i < layerStackUpdateDurations.size(); i++)
            {
                average += layerStackUpdateDurations[i];
            }
            layerStackAverageUpdateDuration = average / layerStackUpdateDurations.size();
        }

        inline static long double GetAppUpdatesPerSecond()
        {
            return 1. / layerStackAverageUpdateDuration.GetSeconds();
        }

        inline static long double GetAppUpdateTimeMS()
        {
            return layerStackAverageUpdateDuration.GetMilliseconds();
        }

    private:
        static Timestamp layerStackAverageUpdateDuration;
        static std::deque<Timestamp> layerStackUpdateDurations;

        friend class App;
    };
}