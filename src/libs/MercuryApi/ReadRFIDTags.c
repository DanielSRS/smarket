/**
 * Sample program that reads tags for a fixed period of time (500ms)
 * and prints the tags found.
 * @file read.c
 */

#include <tm_reader.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <inttypes.h>
#include "ReadRFIDTags.h"

#ifndef BARE_METAL
#if WIN32
#define snprintf sprintf_s
#endif

/* Enable this to use transportListener */
#ifndef USE_TRANSPORT_LISTENER
#define USE_TRANSPORT_LISTENER 0
#endif
#define PRINT_TAG_METADATA 0
#define numberof(x) (sizeof((x))/sizeof((x)[0]))

#define usage() {errx(1, "Please provide valid reader URL, such as: reader-uri [--ant n] [--pow read_power]\n"\
                         "reader-uri : e.g., 'tmr:///COM1' or 'tmr:///dev/ttyS0/' or 'tmr://readerIP'\n"\
                         "[--ant n] : e.g., '--ant 1'\n"\
                         "[--pow read_power] : e.g, '--pow 2300'\n"\
                         "Example for UHF modules: 'tmr:///com4' or 'tmr:///com4 --ant 1,2' or 'tmr:///com4 --ant 1,2 --pow 2300'\n"\
                         "Example for HF/LF modules: 'tmr:///com4' \n");}

void errx(int exitval, const char *fmt, ...)
{
  va_list ap;

  va_start(ap, fmt);
  vfprintf(stderr, fmt, ap);

  exit(exitval);
}

void checkerr(TMR_Reader* rp, TMR_Status ret, int exitval, const char *msg)
{
  if (TMR_SUCCESS != ret)
  {
    errx(exitval, "Error %s: %s\n", msg, TMR_strerr(rp, ret));
  }
}

void serialPrinter(bool tx, uint32_t dataLen, const uint8_t data[],
                   uint32_t timeout, void *cookie)
{
  FILE *out = cookie;
  uint32_t i;

  fprintf(out, "%s", tx ? "Sending: " : "Received:");
  for (i = 0; i < dataLen; i++)
  {
    if (i > 0 && (i & 15) == 0)
    {
      fprintf(out, "\n         ");
    }
    fprintf(out, " %02x", data[i]);
  }
  fprintf(out, "\n");
}

void stringPrinter(bool tx,uint32_t dataLen, const uint8_t data[],uint32_t timeout, void *cookie)
{
  FILE *out = cookie;

  fprintf(out, "%s", tx ? "Sending: " : "Received:");
  fprintf(out, "%s\n", data);
}

void parseAntennaList(uint8_t *antenna, uint8_t *antennaCount, char *args)
{
  char *token = NULL;
  char *str = ",";
  uint8_t i = 0x00;
  int scans;

  /* get the first token */
  if (NULL == args)
  {
    fprintf(stdout, "Missing argument\n");
    usage();
  }

  token = strtok(args, str);
  if (NULL == token)
  {
    fprintf(stdout, "Missing argument after %s\n", args);
    usage();
  }

  while(NULL != token)
  {
    scans = sscanf(token, "%"SCNu8, &antenna[i]);
    if (1 != scans)
    {
      fprintf(stdout, "Can't parse '%s' as an 8-bit unsigned integer value\n", token);
      usage();
    }
    i++;
    token = strtok(NULL, str);
  }
  *antennaCount = i;
}
#endif

int readRFIDTags(int argc, char **argv, Map * result)
{
  TMR_Reader r, *rp;
  TMR_Status ret;
  TMR_ReadPlan plan;
  TMR_Region region;
#define READPOWER_NULL (-12345)
  int readpower = READPOWER_NULL;
#ifndef BARE_METAL
  uint8_t i;
#endif /* BARE_METAL*/
  uint8_t buffer[20];
  uint8_t *antennaList = NULL;
  uint8_t antennaCount = 0x0;
  TMR_TRD_MetadataFlag metadata = TMR_TRD_METADATA_FLAG_ALL;
  char string[100];
  TMR_String model;

#ifndef BARE_METAL
#if USE_TRANSPORT_LISTENER
  TMR_TransportListenerBlock tb;
#endif

  if (argc < 2)
  {
    fprintf(stdout, "Not enough arguments.  Please provide reader URL.\n");
    usage();
  }

  for (i = 2; i < argc; i+=2)
  {
    if(0x00 == strcmp("--ant", argv[i]))
    {
      if (NULL != antennaList)
      {
        fprintf(stdout, "Duplicate argument: --ant specified more than once\n");
        usage();
      }
      parseAntennaList(buffer, &antennaCount, argv[i+1]);
      antennaList = buffer;
    }
    else if (0 == strcmp("--pow", argv[i]))
    {
      long retval;
      char *startptr;
      char *endptr;
      startptr = argv[i+1];
      retval = strtol(startptr, &endptr, 0);
      if (endptr != startptr)
      {
        readpower = retval;
        fprintf(stdout, "Requested read power: %d cdBm\n", readpower);
      }
      else
      {
        fprintf(stdout, "Can't parse read power: %s\n", argv[i+1]);
      }
    }
    else
    {
      fprintf(stdout, "Argument %s is not recognized\n", argv[i]);
      usage();
    }
  }
#endif

  rp = &r;
#ifndef BARE_METAL
  ret = TMR_create(rp, argv[1]);
#else
  ret = TMR_create(rp, "tmr:///com1");
  buffer[0] = 1;
  antennaList = buffer;
  antennaCount = 0x01;
#endif

#ifndef BARE_METAL
  checkerr(rp, ret, 1, "creating reader");

#if USE_TRANSPORT_LISTENER

  if (TMR_READER_TYPE_SERIAL == rp->readerType)
  {
    tb.listener = serialPrinter;
  }
  else
  {
    tb.listener = stringPrinter;
  }
  tb.cookie = stdout;

  TMR_addTransportListener(rp, &tb);
#endif
#endif

  ret = TMR_connect(rp);
#ifndef BARE_METAL
  checkerr(rp, ret, 1, "connecting reader");
#endif

  model.value = string;
  TMR_paramGet(rp, TMR_PARAM_VERSION_MODEL, &model);
#ifndef BARE_METAL
  checkerr(rp, ret, 1, "Getting version model");
#endif

  if (0 != strcmp("M3e", model.value))
  {
    region = TMR_REGION_NONE;
    ret = TMR_paramGet(rp, TMR_PARAM_REGION_ID, &region);
#ifndef BARE_METAL
    checkerr(rp, ret, 1, "getting region");
#endif

    if (TMR_REGION_NONE == region)
    {
      TMR_RegionList regions;
      TMR_Region _regionStore[32];
      regions.list = _regionStore;
      regions.max = sizeof(_regionStore)/sizeof(_regionStore[0]);
      regions.len = 0;

      ret = TMR_paramGet(rp, TMR_PARAM_REGION_SUPPORTEDREGIONS, &regions);
#ifndef BARE_METAL
      checkerr(rp, ret, __LINE__, "getting supported regions");

      if (regions.len < 1)
      {
        checkerr(rp, TMR_ERROR_INVALID_REGION, __LINE__, "Reader doesn't support any regions");
      }
#endif
      region = regions.list[0];
      ret = TMR_paramSet(rp, TMR_PARAM_REGION_ID, &region);
#ifndef BARE_METAL
      checkerr(rp, ret, 1, "setting region");
#endif
    }

    if (READPOWER_NULL != readpower)
    {
      int value;

      ret = TMR_paramGet(rp, TMR_PARAM_RADIO_READPOWER, &value);
#ifndef BARE_METAL
      checkerr(rp, ret, 1, "getting read power");
      printf("Old read power = %d dBm\n", value);
#endif
      value = readpower;
      ret = TMR_paramSet(rp, TMR_PARAM_RADIO_READPOWER, &value);
#ifndef BARE_METAL
      checkerr(rp, ret, 1, "setting read power");
#endif
    }

    {
      int value;
      ret = TMR_paramGet(rp, TMR_PARAM_RADIO_READPOWER, &value);
#ifndef BARE_METAL
      checkerr(rp, ret, 1, "getting read power");
      printf("Read power = %d dBm\n", value);
#endif
    }

#ifdef TMR_ENABLE_UHF
    /**
     * Checking the software version of the sargas.
     * The antenna detection is supported on sargas from software version of 5.3.x.x.
     * If the Sargas software version is 5.1.x.x then antenna detection is not supported.
     * User has to pass the antenna as arguments.
     */
    {
      ret = isAntDetectEnabled(rp, antennaList);
      if(TMR_ERROR_UNSUPPORTED == ret)
      {
#ifndef BARE_METAL
        fprintf(stdout, "Reader doesn't support antenna detection. Please provide antenna list.\n");
        usage();
#endif
      }
      else
      {
#ifndef BARE_METAL
        checkerr(rp, ret, 1, "Getting Antenna Detection Flag Status");
#endif
      }
    }
#endif /* TMR_ENABLE_UHF */
  }
  else
  {
    if (antennaList != NULL)
    {
#ifndef BARE_METAL
      printf("Module doesn't support antenna input\n");
      usage();
#endif /* BARE_METAL */
    }
  }

#ifdef TMR_ENABLE_LLRP_READER
  if (0 != strcmp("Mercury6", model.value))
#endif /* TMR_ENABLE_LLRP_READER */
  {
        // Set the metadata flags. Protocol is mandatory metadata flag and reader don't allow to disable the same
        // metadata = TMR_TRD_METADATA_FLAG_ANTENNAID | TMR_TRD_METADATA_FLAG_FREQUENCY | TMR_TRD_METADATA_FLAG_PROTOCOL;
        ret = TMR_paramSet(rp, TMR_PARAM_METADATAFLAG, &metadata);
#ifndef BARE_METAL
        checkerr(rp, ret, 1, "Setting Metadata Flags");
#endif
  }

  /**
  * for antenna configuration we need two parameters
  * 1. antennaCount : specifies the no of antennas should
  *    be included in the read plan, out of the provided antenna list.
  * 2. antennaList  : specifies  a list of antennas for the read plan.
  **/
  // initialize the read plan
  if (0 != strcmp("M3e", model.value))
  {
    ret = TMR_RP_init_simple(&plan, antennaCount, antennaList, TMR_TAG_PROTOCOL_GEN2, 1000);
  }
  else
  {
    ret = TMR_RP_init_simple(&plan, antennaCount, antennaList, TMR_TAG_PROTOCOL_ISO14443A, 1000);
  }
#ifndef BARE_METAL
  checkerr(rp, ret, 1, "initializing the  read plan");
#endif

  /* Commit read plan */
  ret = TMR_paramSet(rp, TMR_PARAM_READ_PLAN, &plan);
#ifndef BARE_METAL
  checkerr(rp, ret, 1, "setting read plan");
#endif
  ret = TMR_read(rp, 500, NULL);

#ifndef BARE_METAL
if (TMR_ERROR_TAG_ID_BUFFER_FULL == ret)
  {
    /* In case of TAG ID Buffer Full, extract the tags present
    * in buffer.
    */
    fprintf(stdout, "reading tags:%s\n", TMR_strerr(rp, ret));
  }
  else
  {
    checkerr(rp, ret, 1, "reading tags");
  }
#endif
  while (TMR_SUCCESS == TMR_hasMoreTags(rp))
  {
    TMR_TagReadData trd;
    char idStr[128];
#ifndef BARE_METAL
    char timeStr[128];
#endif /* BARE_METAL */

    ret = TMR_getNextTag(rp, &trd);
#ifndef BARE_METAL
  checkerr(rp, ret, 1, "fetching tag");
#endif
    TMR_bytesToHex(trd.tag.epc, trd.tag.epcByteCount, idStr);

#ifndef BARE_METAL
#ifdef WIN32
                if (rp->readerType == TMR_READER_TYPE_SERIAL)
                {
                        FILETIME ft, utc;
                        SYSTEMTIME st;
                        char* timeEnd;
                        char* end;

                        utc.dwHighDateTime = trd.timestampHigh;
                        utc.dwLowDateTime = trd.timestampLow;

                        FileTimeToLocalFileTime( &utc, &ft );
                        FileTimeToSystemTime( &ft, &st );
                        timeEnd = timeStr + sizeof(timeStr)/sizeof(timeStr[0]);
                        end = timeStr;
                        end += sprintf(end, "%d-%d-%d", st.wYear,st.wMonth,st.wDay);
                        end += sprintf(end, "T%d:%d:%d.%03d", st.wHour,st.wMinute,st.wSecond, st.wMilliseconds);
                }
                else
                {
                        uint8_t shift;
                        uint64_t timestamp;
                        time_t seconds;
                        int micros;
                        char* timeEnd;
                        char* end;

                        shift = 32;
                        timestamp = ((uint64_t)trd.timestampHigh<<shift) | trd.timestampLow;
                        seconds = timestamp / 1000;
                        micros = (int) ((timestamp % 1000) * 1000);

                        /*
                        * Timestamp already includes millisecond part of dspMicros,
                        * so subtract this out before adding in dspMicros again
                        */
                        micros -= trd.dspMicros / 1000;
                        micros += trd.dspMicros;

                        timeEnd = timeStr + sizeof(timeStr)/sizeof(timeStr[0]);
                        end = timeStr;
                        end += strftime(end, timeEnd-end, "%Y-%m-%dT%H:%M:%S", localtime(&seconds));
                        end += snprintf(end, timeEnd-end, ".%06d", micros);
                        //  end += strftime(end, timeEnd-end, "%z", localtime(&seconds));
                }
#else
    {
      uint8_t shift;
      uint64_t timestamp;
      time_t seconds;
      int micros;
      char* timeEnd;
      char* end;

      shift = 32;
      timestamp = ((uint64_t)trd.timestampHigh<<shift) | trd.timestampLow;
      seconds = timestamp / 1000;
      micros = (timestamp % 1000) * 1000;

      /*
       * Timestamp already includes millisecond part of dspMicros,
       * so subtract this out before adding in dspMicros again
       */
      micros -= trd.dspMicros / 1000;
      micros += trd.dspMicros;

      timeEnd = timeStr + sizeof(timeStr)/sizeof(timeStr[0]);
      end = timeStr;
      end += strftime(end, timeEnd-end, "%Y-%m-%dT%H:%M:%S", localtime(&seconds));
      end += snprintf(end, timeEnd-end, ".%06d", micros);
      end += strftime(end, timeEnd-end, "%z", localtime(&seconds));
    }
#endif

  printf("Tag ID: %s ", idStr);
  result->setString(result, idStr, idStr);

// Enable PRINT_TAG_METADATA Flags to print Metadata value
#if PRINT_TAG_METADATA
{
  uint16_t j = 0;
  printf("\n");
  for (j=0; (1<<j) <= TMR_TRD_METADATA_FLAG_MAX; j++)
        {
                if ((TMR_TRD_MetadataFlag)trd.metadataFlags & (1<<j))
                {
                        switch ((TMR_TRD_MetadataFlag)trd.metadataFlags & (1<<j))
                        {
                                case TMR_TRD_METADATA_FLAG_READCOUNT:
                                        printf("Read Count: %d\n", trd.readCount);
                                        break;
                                case TMR_TRD_METADATA_FLAG_ANTENNAID:
                                        printf("Antenna ID: %d\n", trd.antenna);
                                        break;
                                case TMR_TRD_METADATA_FLAG_TIMESTAMP:
                                        printf("Timestamp: %s\n", timeStr);
                                        break;
                                case TMR_TRD_METADATA_FLAG_PROTOCOL:
                                        printf("Protocol: %d\n", trd.tag.protocol);
                                        break;
#ifdef TMR_ENABLE_UHF
                                case TMR_TRD_METADATA_FLAG_RSSI:
                                        printf("RSSI: %d\n", trd.rssi);
                                        break;
                                case TMR_TRD_METADATA_FLAG_FREQUENCY:
                                        printf("Frequency: %d\n", trd.frequency);
                                        break;
                                case TMR_TRD_METADATA_FLAG_PHASE:
                                        printf("Phase: %d\n", trd.phase);
                                        break;
#endif /* TMR_ENABLE_UHF */
                                case TMR_TRD_METADATA_FLAG_DATA:
                                        //TODO : Initialize Read Data
                                        if (0 < trd.data.len)
                                        {
                                                char dataStr[255];
                                                TMR_bytesToHex(trd.data.list, trd.data.len, dataStr);
                                                printf("Data(%d): %s\n", trd.data.len, dataStr);
                                        }
                                        break;
#ifdef TMR_ENABLE_UHF
                                case TMR_TRD_METADATA_FLAG_GPIO_STATUS:
                    {
                      if (rp->readerType == TMR_READER_TYPE_SERIAL)
                      {
                        printf("GPI status:\n");
                        for (i = 0 ; i < trd.gpioCount ; i++)
                        {
                          printf("Pin %d: %s\n", trd.gpio[i].id, trd.gpio[i].bGPIStsTagRdMeta ? "High" : "Low");
                        }
                        printf("GPO status:\n");
                        for (i = 0 ; i < trd.gpioCount ; i++)
                        {
                          printf("Pin %d: %s\n", trd.gpio[i].id, trd.gpio[i].high ? "High" : "Low");
                        }
                      }
                      else
                      {
                        printf("GPI status:\n");
                        for (i = 0 ; i < trd.gpioCount/2 ; i++)
                        {
                          printf("Pin %d: %s\n", trd.gpio[i].id, trd.gpio[i].high ? "High" : "Low");
                        }
                        printf("GPO status:\n");
                        for (i = trd.gpioCount/2 ; i < trd.gpioCount ; i++)
                        {
                          printf("Pin %d: %s\n", trd.gpio[i].id, trd.gpio[i].high ? "High" : "Low");
                        }
                        }
                    }
                    break;
        if (TMR_TAG_PROTOCOL_GEN2 == trd.tag.protocol)
        {
          case TMR_TRD_METADATA_FLAG_GEN2_Q:
            printf("Gen2Q: %d\n", trd.u.gen2.q.u.staticQ.initialQ);
            break;
          case TMR_TRD_METADATA_FLAG_GEN2_LF:
            {
              printf("Gen2Linkfrequency: ");
              switch(trd.u.gen2.lf)
              {
                case TMR_GEN2_LINKFREQUENCY_250KHZ:
                  printf("250(khz)\n");
                  break;
                case TMR_GEN2_LINKFREQUENCY_320KHZ:
                  printf("320(khz)\n");
                  break;
                case TMR_GEN2_LINKFREQUENCY_640KHZ:
                  printf("640(khz)\n");
                  break;
                default:
                  printf("Unknown value(%d)\n",trd.u.gen2.lf);
                  break;
              }
              break;
            }
          case TMR_TRD_METADATA_FLAG_GEN2_TARGET:
            {
              printf("Gen2Target: ");
              switch(trd.u.gen2.target)
              {
                case TMR_GEN2_TARGET_A:
                  printf("A\n");
                  break;
                case TMR_GEN2_TARGET_B:
                  printf("B\n");
                  break;
                default:
                  printf("Unknown Value(%d)\n",trd.u.gen2.target);
                  break;
              }
              break;
            }
        }
#endif /* TMR_ENABLE_UHF */
#ifdef TMR_ENABLE_HF_LF
          case TMR_TRD_METADATA_FLAG_TAGTYPE:
            {
              printf("TagType: 0x%08x\n", trd.tagType);
              break;
            }
#endif /* TMR_ENABLE_HF_LF */
                                default:
                                        break;
                        }
                }
        }
}
#endif
        printf ("\n");
#endif
  }

  TMR_destroy(rp);
  return 0;
}
