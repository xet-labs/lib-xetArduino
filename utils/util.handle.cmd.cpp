#include "utils.h"
#include "../lib.h"

namespace handle
{
  __attribute__((weak)) void cmd(const char *cmd, char *const args[], size_t argc)
  {
    uint32_t hash = fnv1a(cmd);

    switch (hash)
    {
    case fnv1a("echo"):
      for (size_t i = 0; i < argc; ++i)
      {
        Serial.print(args[i]);
        Serial.print(' ');
      }
      Serial.println();
      break;

    case fnv1a("lgc"):
      if (argc > 0)
      {
        switch (fnv1a(args[0]))
        {
        case fnv1a("assign"):
          lgc::assign(args[1]);
          break;

        case fnv1a("list"):
          lgc::list();
          break;

        default:
          Serial.print("Unhandled subcommand: ");
          Serial.println(args[0]);
          break;
        }
      }
      break;

    case fnv1a("led"):
      if (argc > 0)
      {
        if (strcmp(args[0], "on") == 0)
        {
          digitalWrite(sys::hw.LED, HIGH);
          Serial.println("LED turned ON");
        }
        else if (strcmp(args[0], "off") == 0)
        {
          digitalWrite(sys::hw.LED, LOW);
          Serial.println("LED turned OFF");
        }
      }
      break;

    case fnv1a("sta"):
      if (argc > 0)
      {
        const char *sub = args[0];
        char **subArgs = const_cast<char **>(args + 1);
        size_t subArgc = argc - 1;

        switch (fnv1a(sub))
        {
        case fnv1a("info"):
          net::sta::info();
          break;

        case fnv1a("init"):
          switch (subArgc)
          {
          case 0:
            net::sta::init();
            break;
          case 1:
            net::sta::init((char *const[]){subArgs[0], nullptr});
            break;

          case 2:
            net::sta::init((char *const[]){subArgs[0], subArgs[1], nullptr});
            break;

          default:
            Serial.println("Too many arguments for 'sta init'");
            break;
          }
          break;

        case fnv1a("scan"):
          net::sta::scan();
          break;

        default:
          Serial.print("Unhandled subcommand: ");
          Serial.println(sub);
          break;
        }
      }
      break;

    case fnv1a("sys"):
      if (argc > 0)
      {
        switch (fnv1a(args[0]))
        {
        case fnv1a("info"):
          sys::info();
          break;

        case fnv1a("lspart"):
          sys::lspart();
          break;

        default:
          Serial.print("Unhandled subcommand: ");
          Serial.println(args[0]);
          break;
        }
      }
      break;

    default:
      Serial.print("Unhandled command: ");
      Serial.println(cmd);
      break;
    }
  }
}