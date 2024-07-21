#include "Anwill.h"
#include "EditorLayer.h"
#include "base/Sandbox.h"

namespace Anwill {

    class AnwillEditor : public App
    {
    public:
        AnwillEditor(const Anwill::AppSettings& settings)
            : App(settings)
        {

        }

    private:
        WindowSettings windowSettings;
    };
}

Anwill::App* Anwill::CreateApp()
{
    // Define graphics API, Window Settings and more
    // TODO: Move to local config file perhaps
    Anwill::AppSettings appSettings = AppSettings()
        .SetTitle("Anwill Editor")
        .SetWindowResolution(1800, 900)
        .SetGraphicsAPI(GraphicsAPI::API::OpenGL)
        .SetNumSamples(8);

    //return new AnwillEditor(appSettings);
    return new Sandbox(appSettings);
}

#include "EntryPoint.h" // Get the main function