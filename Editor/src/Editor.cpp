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
    Anwill::AppSettings appSettings = AppSettings()
        .SetTitle("Anwill Editor")
        .SetWindowResolution(1920, 1080)
        .SetGraphicsAPI(GraphicsAPI::API::OpenGL)
        .SetNumSamples(8);

    //return new AnwillEditor(appSettings);
    return new Sandbox(appSettings);
}

#include "EntryPoint.h" // Get the main function