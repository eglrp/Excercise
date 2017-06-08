#include <string>
#include "Aria.h"


/*
  �������һ���ص�����������Щ�ص�������صĺ��ӽ��ᱻ���ݸ�DriverClass
*/
class CallbackContainer
{
public:

  void callback1();
  void callback2(int i);
  bool callback3(const char *str);
};

void CallbackContainer::callback1()
{
  printf("CallbackContainer::callback1 called.\n");
}

void CallbackContainer::callback2(int i)
{
  printf("CallbackContainer::callback2 called with argument of '%d'\n", i);
}

bool CallbackContainer::callback3(const char *str)
{
  printf("CallbackContainer::callback3 called with argument of '%s'.\n", str);
  return(true);
}

/* 
 *����Ҳ���Ե���ȫ�ֺ���
 */

void globalCallback()
{
  printf("globalCallback() called.\n");
}


/*
  ����һ������ʻ���࣬�������ֲ�ͬ���͵Ĺ��ܣ�����������ֺ��ӡ�
  ���Ǻ��ӵĵ�������������ɢ��ϵĶ���֮�䴫����Ϣ���¼�֪ͨ��
*/
class DriverClass
{
public:

  void invokeFunctors();

  void setCallback1(ArFunctor *func) {myFunc1=func;}
  void setCallback2(ArFunctor1<int> *func) {myFunc2=func;}
  void setCallback3(ArRetFunctor1<bool, const char *> *func) {myFunc3=func;}


protected:

  ArFunctor *myFunc1;
  ArFunctor1<int> *myFunc2;
  ArRetFunctor1<bool, const char *> *myFunc3;
};

void DriverClass::invokeFunctors()
{
  bool ret;

  printf("Invoking functor1... ");
  myFunc1->invoke();

  printf("Invoking functor2... ");
  myFunc2->invoke(23);
     

  /*
     ���з���ֵ�ĺ��ӣ�ʹ��invorkeR()����ȡ����ֵ��
	 invorke()Ҳ�ܵ��ô��ຯ�ӣ����޷���ȡ����ֵ��(�Է���ֵ��ָ��ĺ��ӿ�������ڴ�й©)
  */
  printf("Invoking functor3... ");
  ret=myFunc3->invokeR("This is a string argument");
  if (ret)
    printf("\t-> functor3 returned 'true'\n");
  else
    printf("\t-> functor3 returned 'false'\n");
}

int main()
{
  CallbackContainer cb;
  DriverClass driver;

  ArFunctorC<CallbackContainer> functor1(cb, &CallbackContainer::callback1);
  ArFunctor1C<CallbackContainer, int> functor2(cb, &CallbackContainer::callback2);
  ArRetFunctor1C<bool, CallbackContainer, const char *>
    functor3(cb, &CallbackContainer::callback3);

  driver.setCallback1(&functor1);
  driver.setCallback2(&functor2);
  driver.setCallback3(&functor3);

  driver.invokeFunctors();

  /* ���Խ�����ָ��ȫ�ֺ��� */
  ArGlobalFunctor globalFunctor(&globalCallback);
  printf("Invoking globalFunctor... ");
  globalFunctor.invoke();

  /* �����ڻص�ʱ���������ʹ�õ��øú���ʱ��Ϊ��ֵͬ
   */
  ArFunctor1C<CallbackContainer, int> functor4(cb, &CallbackContainer::callback2, 42);
  printf("Invoking functor with constant argument... ");
  functor4.invoke();

  /* �������Ա����ӿ���ʵ������ֻҪ���ǵĵ��ò���Ҫ������
   */
  ArFunctor* baseFunctor = &functor4;
  printf("Invoking downcast functor... ");
  baseFunctor->invoke();


  return(0);
}