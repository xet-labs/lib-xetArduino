#include "utils.h"
#include "../lib.h"
#include <string.h>

#if __has_include("../../lgc/lgc.h")
#include "../../lgc/lgc.h"
#define LGC_ENABLED
#endif

namespace handle
{
  String serialBuffer;

  void serial()
  {
    while (Serial.available())
    {
      char ch = Serial.read();

      switch (ch)
      {
      case '\r':
        break;

      case '\n':
        serialBuffer.trim();
        if (serialBuffer.length())
        {
          constexpr int MAX_ARGS = 8;
          char *argv[MAX_ARGS] = {nullptr};
          int argc = 0;

          // convert String to modifiable buffer
          char *buf = serialBuffer.begin();
          int len = serialBuffer.length();

          for (int i = 0; i <= len && argc < MAX_ARGS; ++i)
          {
            if (i == len || buf[i] == ' ')
            {
              buf[i] = '\0';
              if (argv[argc])
                argc++;
              argv[argc] = &buf[i + 1];
            }
            else if (!argv[argc])
            {
              argv[argc] = &buf[i];
            }
          }

          if (argc > 0)
            cmd(argv[0], argv + 1, argc - 1);
        }

        serialBuffer.clear();
        Serial.print("\n> ");
        break;

      case '\b':
      case 127:
        if (!serialBuffer.isEmpty())
        {
          serialBuffer.remove(serialBuffer.length() - 1);
          Serial.print("\b \b");
        }
        break;

      default:
        if (isPrintable(ch))
        {
          serialBuffer += ch;
          // Serial.print(ch);
        }
        break;
      }
    }
  }
}