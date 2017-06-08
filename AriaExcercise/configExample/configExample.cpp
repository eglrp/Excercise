#include "Aria.h"

//��ʾʹ��ArConfig��ʾ������˳�����ʾ���ʹ��ArConfig���洢���ò��������ļ�����/�������ǡ�

class ConfigExample
{
  ArConfig* myConfig;
  int myIntParam;
  double myDoubleParam;
  bool myBoolParam;
  char myStringParam[256];
  ArRetFunctorC<bool, ConfigExample> myProcessConfigCB;

public:
  ConfigExample():
    myIntParam(0),
    myDoubleParam(0.5),
    myBoolParam(false),
    myProcessConfigCB(this, &ConfigExample::processConfigFile)
  {
    // ȫ��Aria���а���һ��ArConfig���󣬿��Դ���һ����ArConfig����
	// ��ʹ��Ĭ�϶�������ڲ�ͬ����������
    // ��ArConfig�д洢һ�����ò���ʱ������Ҫ��Ҫ����ӵ�ArConfig�����С�
	// �����Ƿֿ�洢�ģ�ͨ��һ��ArConfigArg�����ڵ�ָ��Ӱ�����
    ArConfig* config = Aria::getConfig();
	// Ϊһ�����������
    config->setSectionComment("Example Section", "Contains parameters created by the configExample");

    // ���һ��-10��10����������
	// ArConfigArg��������ֱ�Ϊ����������ָ�룬��������Сֵ�����ֵ
	// addParamc����Ϊ��ArConfigArg���󣬿��������ȼ�
    config->addParam( ArConfigArg("ExampleIntegerParameter", &myIntParam, "Example parameter integer.", -10, 10), "Example Section", ArPriority::NORMAL);
    
    // ���һ��-0.0��1.0��С˵����
    config->addParam( ArConfigArg("ExampleDoubleParameter", &myDoubleParam, "Example double precision floating point number.", 0.0, 1.0), "Example Section", ArPriority::NORMAL);

    // �����������Ե������ȼ�
    config->addParam( ArConfigArg("ExampleBoolParameter", &myBoolParam, "Example boolean parameter."), "Example Section", ArPriority::IMPORTANT);

    // ����Ҫ�������Ե��������ȼ�
    myStringParam[0] = '\0';  // �ַ����ÿ�
    config->addParam( ArConfigArg("ExampleStringParameter", myStringParam, "Example string parameter.", 256), "Example Section", ArPriority::TRIVIAL);

    // ���������޸�ʱ�Ļص��������Է���һ��Ҫ�Բ����ı��л�Ӧʱʹ��
    config->addProcessFileCB(&myProcessConfigCB, 0);		//�ڶ�����Ϊ���ȼ�
  }

  
  // ���������ļ��������øı�ʱ���õķ���������true��false��ָʾ�Ƿ����
  bool processConfigFile() 
  {
    ArLog::log(ArLog::Normal, "configExample: Config changed. New values: int=%d, float=%f, bool=%s, string=\"%s\".", myIntParam, myDoubleParam, myBoolParam?"true":"false", myStringParam);
    return true;
  }
};
  
int main(int argc, char **argv)
{
  Aria::init();
  ArArgumentParser argParser(&argc, argv);
  argParser.loadDefaultArguments();
  if (argc < 2 || !Aria::parseArgs() || argParser.checkArgument("-help"))
  {
    ArLog::log(ArLog::Terse, "configExample usage: configExample <config file>.\nFor example, \"configExample examples/configExample.cfg\".");
    Aria::logOptions();
    Aria::exit(1);
    return 1;
  }
  
  
  ConfigExample configExample;

  // ͨ��Aria����һ�������ļ���ArConfig������
  // ͨ�����������λ��������ARIAĿ¼�У�����/usr/local/Aria��ͨ��$ARIA��������ָ����Ŀ¼�У�
  char error[512];
  const char* filename = argParser.getArg(1);
  ArConfig* config = Aria::getConfig();
  ArLog::log(ArLog::Normal, "configExample: loading configuration file \"%s\"...", filename);
  if (! config->parseFile(filename, true, false, error, 512) )
  {
    ArLog::log(ArLog::Terse, "configExample: Error loading configuration file \"%s\" %s. Try \"examples/configExample.cfg\".", filename, error);
    Aria::exit(-1);
    return -1;
  }

  ArLog::log(ArLog::Normal, "configExample: Loaded configuration file \"%s\".", filename);
  
  // �ڸı�һ�����õ�ʲ�󣬵��ûص�����
  ArConfigSection* section = config->findSection("Example Section");
  if (section)
  {
    ArConfigArg* arg = section->findParam("ExampleBoolParameter");
    if (arg)
    {
      arg->setBool(!arg->getBool());
      if (! config->callProcessFileCallBacks(false, error, 512) )
      {
        ArLog::log(ArLog::Terse, "configExample: Error processing modified config: %s.", error);
      }
      else
      {
        ArLog::log(ArLog::Normal, "configExample: Successfully modified config and invoked callbacks.");
      }
    }
  }

  // ���������ļ�
  ArLog::log(ArLog::Normal, "configExample: Saving configuration...");
  if(!config->writeFile(filename))
  {
    ArLog::log(ArLog::Terse, "configExample: Error saving configuration to file \"%s\"!", filename);
  }

  // ��������
  ArLog::log(ArLog::Normal, "configExample: end of program.");
  Aria::exit(0);
  return 0;
}