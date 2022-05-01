
#include "../constants.h"
#include "../CLI/cli.h"
#include "../settings.h"

void IDNCommand(CliHandle_t *handle, void *v)
{
    if (handle->LastRead == '?')
    {
        WriteString(handle, MakeString);
        WriteChar(handle, ',');

        WriteString(handle, ModelString);
        WriteChar(handle, ',');

        QuietSettings_t settings = GetSettings();
        char *snc = settings.SerialNumber;
        while (*snc != 0x00)
            handle->Write(*snc++);
        WriteChar(handle, ',');

        WriteString(handle, VersionString);
    }
}

void RSTCommand(CliHandle_t *handle, void *v)
{
    ClearAllErrors();
    RestoreSettings(false);
}

CommandDefinition_t starCommands[] = {
    DEFINE_COMMAND("IDN", IDNCommand),
    DEFINE_COMMAND("RST", RSTCommand),
};

CommandDefinition_t StarCommand = DEFINE_BRANCH("*", starCommands);
