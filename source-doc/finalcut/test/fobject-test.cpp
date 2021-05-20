/***********************************************************************
* fobject-test.cpp - FPoint unit tests                                 *
*                                                                      *
* This file is part of the FINAL CUT widget toolkit                    *
*                                                                      *
* Copyright 2018-2020 Markus Gans                                      *
*                                                                      *
* FINAL CUT is free software; you can redistribute it and/or modify    *
* it under the terms of the GNU Lesser General Public License as       *
* published by the Free Software Foundation; either version 3 of       *
* the License, or (at your option) any later version.                  *
*                                                                      *
* FINAL CUT is distributed in the hope that it will be useful, but     *
* WITHOUT ANY WARRANTY; without even the implied warranty of           *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        *
* GNU Lesser General Public License for more details.                  *
*                                                                      *
* You should have received a copy of the GNU Lesser General Public     *
* License along with this program.  If not, see                        *
* <http://www.gnu.org/licenses/>.                                      *
***********************************************************************/

#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>

#include <final/final.h>

namespace test
{

//----------------------------------------------------------------------
// class FObject_protected
//----------------------------------------------------------------------

class FObject_protected : public finalcut::FObject
{
  public:
    FObject_protected()
      : count(0)
    { }

    bool event (finalcut::FEvent* ev)
    {
      return finalcut::FObject::event(ev);
    }

    FTimerList* getTimerList() const
    {
      return finalcut::FObject::getTimerList();
    }

    uInt processEvent()
    {
      return processTimerEvent();
    }

    void setWidgetProperty (bool property)
    {
      finalcut::FObject::setWidgetProperty (property);
    }

    bool isWidget()
    {
      return finalcut::FObject::isWidget();
    }

    virtual void performTimerAction (FObject*, finalcut::FEvent*)
    {
      std::cout << ".";
      fflush(stdout);
      count++;
    }

    // Data member
    uInt count;
};

//----------------------------------------------------------------------

class FObject_timer : public finalcut::FObject
{
  public:
    FObject_timer()
    { }

    int getValue() const
    {
      return value;
    }

  protected:
    virtual void onTimer (finalcut::FTimerEvent* ev)
    {
      if ( ev->getTimerId() == 1 )
        value++;
    }

  private:
    // Data member
    int value{0};
};

//----------------------------------------------------------------------

class FObject_userEvent : public finalcut::FObject
{
  public:
    FObject_userEvent()
    { }

    int getValue() const
    {
      return value;
    }

  protected:
    virtual void onUserEvent (finalcut::FUserEvent* ev)
    {
      if ( ev->getUserId() == 42 )
      {
        value = ev->getData<int>();

        if ( ev->getFDataObject<int>().isInitializedReference() )
          ev->getData<int>()++;  // this has external effects
      }
    }

  private:
    // Data member
    int value{0};
};

}  // namespace test


//----------------------------------------------------------------------
// class FObjectTest
//----------------------------------------------------------------------

class FObjectTest : public CPPUNIT_NS::TestFixture
{
  public:
    FObjectTest()
    { }

  protected:
    void classNameTest();
    void noArgumentTest();
    void childObjectTest();
    void widgetObjectTest();
    void removeParentTest();
    void setParentTest();
    void addTest();
    void delTest();
    void elementAccessTest();
    void iteratorTest();
    void timeTest();
    void timerTest();
    void performTimerActionTest();
    void userEventTest();

  private:
    // Adds code needed to register the test suite
    CPPUNIT_TEST_SUITE (FObjectTest);

    // Add a methods to the test suite
    CPPUNIT_TEST (classNameTest);
    CPPUNIT_TEST (noArgumentTest);
    CPPUNIT_TEST (childObjectTest);
    CPPUNIT_TEST (widgetObjectTest);
    CPPUNIT_TEST (removeParentTest);
    CPPUNIT_TEST (setParentTest);
    CPPUNIT_TEST (addTest);
    CPPUNIT_TEST (delTest);
    CPPUNIT_TEST (elementAccessTest);
    CPPUNIT_TEST (iteratorTest);
    CPPUNIT_TEST (timeTest);
    CPPUNIT_TEST (timerTest);
    CPPUNIT_TEST (performTimerActionTest);
    CPPUNIT_TEST (userEventTest);

    // End of test suite definition
    CPPUNIT_TEST_SUITE_END();
};

//----------------------------------------------------------------------
void FObjectTest::classNameTest()
{
  finalcut::FObject o;
  const finalcut::FString& classname = o.getClassName();
  CPPUNIT_ASSERT ( classname == "FObject" );
}

//----------------------------------------------------------------------
void FObjectTest::noArgumentTest()
{
  finalcut::FObject o1;
  finalcut::FObject o2;
  CPPUNIT_ASSERT ( ! o1.hasParent() );
  CPPUNIT_ASSERT ( o1.getParent() == 0 );
  CPPUNIT_ASSERT ( ! o1.hasChildren() );
  CPPUNIT_ASSERT ( o1.getChild(0) == 0 );
  CPPUNIT_ASSERT ( o1.getChild(1) == 0 );
  CPPUNIT_ASSERT ( o1.numOfChildren() == 0 );
  auto& children_list = o1.getChildren();
  CPPUNIT_ASSERT ( children_list.begin() == o1.begin() );
  CPPUNIT_ASSERT ( children_list.begin() == o1.end() );
  CPPUNIT_ASSERT ( children_list.end() == o1.begin() );
  CPPUNIT_ASSERT ( children_list.end() == o1.end() );
  CPPUNIT_ASSERT ( ! o1.isChild(&o2) );
  CPPUNIT_ASSERT ( ! o1.isDirectChild(&o2) );
  CPPUNIT_ASSERT ( ! o1.isWidget() );
  CPPUNIT_ASSERT ( o1.isInstanceOf("FObject") );
  CPPUNIT_ASSERT ( ! o1.isTimerInUpdating() );

  test::FObject_protected t;
  auto ev = new finalcut::FEvent(finalcut::Event::None);
  CPPUNIT_ASSERT ( ! t.event(ev) );
  delete ev;

  ev = new finalcut::FEvent(finalcut::Event::Timer);
  CPPUNIT_ASSERT ( t.event(ev) );
  delete ev;

  CPPUNIT_ASSERT ( ! finalcut::fc::emptyFString::get().isNull() );
  CPPUNIT_ASSERT ( finalcut::fc::emptyFString::get().isEmpty() );
}

//----------------------------------------------------------------------
void FObjectTest::childObjectTest()
{
  //  obj -> c1 -> c5 -> c6
  //      -> c2
  //      -> c3
  //      -> c4

  finalcut::FObject obj;
  auto c1 = new finalcut::FObject(&obj);
  auto c2 = new finalcut::FObject(&obj);
  auto c3 = new finalcut::FObject(&obj);
  auto c4 = new finalcut::FObject(&obj);
  auto c5 = new finalcut::FObject(c1);
  auto c6 = new finalcut::FObject(c5);
  auto c7 = new finalcut::FObject();

  CPPUNIT_ASSERT ( obj.hasChildren() );
  CPPUNIT_ASSERT ( obj.getChild(0) == 0 );
  CPPUNIT_ASSERT ( obj.getChild(1) != 0 );
  CPPUNIT_ASSERT ( obj.numOfChildren() == 4 );

  CPPUNIT_ASSERT ( obj.isChild(c1) );
  CPPUNIT_ASSERT ( obj.isChild(c2) );
  CPPUNIT_ASSERT ( obj.isChild(c3) );
  CPPUNIT_ASSERT ( obj.isChild(c4) );
  CPPUNIT_ASSERT ( obj.isChild(c5) );
  CPPUNIT_ASSERT ( obj.isChild(c6) );

  CPPUNIT_ASSERT ( obj.isDirectChild(c1) );
  CPPUNIT_ASSERT ( obj.isDirectChild(c2) );
  CPPUNIT_ASSERT ( obj.isDirectChild(c3) );
  CPPUNIT_ASSERT ( obj.isDirectChild(c4) );
  CPPUNIT_ASSERT ( ! obj.isDirectChild(c5) );
  CPPUNIT_ASSERT ( c1->isDirectChild(c5) );
  CPPUNIT_ASSERT ( ! obj.isDirectChild(c6) );
  CPPUNIT_ASSERT ( ! c1->isDirectChild(c6) );
  CPPUNIT_ASSERT ( c5->isDirectChild(c6) );

  CPPUNIT_ASSERT ( c1->hasParent() );
  CPPUNIT_ASSERT ( c1->getParent() == &obj );
  CPPUNIT_ASSERT ( c1->hasChildren() );
  CPPUNIT_ASSERT ( ! c2->hasChildren() );
  CPPUNIT_ASSERT ( c1->getChild(0) == 0 );
  CPPUNIT_ASSERT ( c1->getChild(1) != 0 );
  CPPUNIT_ASSERT ( c2->getChild(1) == 0 );
  CPPUNIT_ASSERT ( c1->numOfChildren() == 1 );
  CPPUNIT_ASSERT ( c2->numOfChildren() == 0 );
  const auto& children_list2 = c1->getChildren();
  CPPUNIT_ASSERT ( children_list2.begin() == c1->begin() );
  CPPUNIT_ASSERT ( children_list2.begin() != c1->end() );
  CPPUNIT_ASSERT ( children_list2.end() != c1->begin() );
  CPPUNIT_ASSERT ( children_list2.end() == c1->end() );
  CPPUNIT_ASSERT ( ! c1->isDirectChild(c7) );
  CPPUNIT_ASSERT ( ! c1->isWidget() );
  CPPUNIT_ASSERT ( c1->isInstanceOf("FObject") );
  CPPUNIT_ASSERT ( ! c1->isTimerInUpdating() );
}

//----------------------------------------------------------------------
void FObjectTest::widgetObjectTest()
{
  test::FObject_protected o;
  CPPUNIT_ASSERT ( ! o.isWidget() );
  o.setWidgetProperty (true);
  CPPUNIT_ASSERT ( o.isWidget() );
  o.setWidgetProperty (false);
  CPPUNIT_ASSERT ( ! o.isWidget() );
}

//----------------------------------------------------------------------
void FObjectTest::removeParentTest()
{
  // obj -> child

  auto obj =  new finalcut::FObject();
  auto child = new finalcut::FObject(obj);

  CPPUNIT_ASSERT ( obj->hasChildren() );
  CPPUNIT_ASSERT ( obj->numOfChildren() == 1 );
  CPPUNIT_ASSERT ( obj->isChild(child) );

  CPPUNIT_ASSERT ( child->hasParent() );
  CPPUNIT_ASSERT ( child->getParent() == obj );

  child->removeParent();
  CPPUNIT_ASSERT ( ! obj->hasChildren() );
  CPPUNIT_ASSERT ( obj->numOfChildren() == 0 );
  CPPUNIT_ASSERT ( ! obj->isChild(child) );

  CPPUNIT_ASSERT ( ! child->hasParent() );
  CPPUNIT_ASSERT ( child->getParent() != obj );

  delete child;
  delete obj;  // also deletes the child object
}

//----------------------------------------------------------------------
void FObjectTest::setParentTest()
{
  // obj -> child
  // => newobj -> child

  auto obj =  new finalcut::FObject();
  auto child = new finalcut::FObject(obj);

  CPPUNIT_ASSERT ( obj->hasChildren() );
  CPPUNIT_ASSERT ( obj->numOfChildren() == 1 );
  CPPUNIT_ASSERT ( obj->isChild(child) );
  CPPUNIT_ASSERT ( obj->isDirectChild(child) );

  CPPUNIT_ASSERT ( child->hasParent() );
  CPPUNIT_ASSERT ( child->getParent() == obj );

  auto newobj =  new finalcut::FObject();

  CPPUNIT_ASSERT ( ! newobj->hasChildren() );
  CPPUNIT_ASSERT ( newobj->numOfChildren() == 0 );
  CPPUNIT_ASSERT ( ! newobj->isChild(child) );
  CPPUNIT_ASSERT ( ! newobj->isDirectChild(child) );

  child->setParent(newobj);

  CPPUNIT_ASSERT ( ! obj->hasChildren() );
  CPPUNIT_ASSERT ( obj->numOfChildren() == 0 );
  CPPUNIT_ASSERT ( ! obj->isChild(child) );
  CPPUNIT_ASSERT ( ! obj->isDirectChild(child) );

  CPPUNIT_ASSERT ( newobj->hasChildren() );
  CPPUNIT_ASSERT ( newobj->numOfChildren() == 1 );
  CPPUNIT_ASSERT ( newobj->isChild(child) );
  CPPUNIT_ASSERT ( newobj->isDirectChild(child) );

  CPPUNIT_ASSERT ( child->hasParent() );
  CPPUNIT_ASSERT ( child->getParent() != obj );
  CPPUNIT_ASSERT ( child->getParent() == newobj );

  delete obj;
  delete child;
  delete newobj;  // also deletes the child object
}

//----------------------------------------------------------------------
void FObjectTest::addTest()
{
  // obj -> child

  auto obj1 =  new finalcut::FObject();
  auto child = new finalcut::FObject();

  CPPUNIT_ASSERT ( ! obj1->hasChildren() );
  CPPUNIT_ASSERT ( obj1->numOfChildren() == 0 );
  CPPUNIT_ASSERT ( ! obj1->isChild(child) );

  CPPUNIT_ASSERT ( ! child->hasParent() );
  CPPUNIT_ASSERT ( child->getParent() != obj1 );

  obj1->addChild(child);
  CPPUNIT_ASSERT ( obj1->hasChildren() );
  CPPUNIT_ASSERT ( obj1->numOfChildren() == 1 );
  CPPUNIT_ASSERT ( obj1->isChild(child) );

  CPPUNIT_ASSERT ( child->hasParent() );
  CPPUNIT_ASSERT ( child->getParent() == obj1 );

  // Switch of the parent by a second addChild
  auto obj2 = new finalcut::FObject();
  obj2->addChild(child);
  CPPUNIT_ASSERT ( child->hasParent() );
  CPPUNIT_ASSERT ( ! obj1->hasChildren() );
  CPPUNIT_ASSERT ( obj1->numOfChildren() == 0 );
  CPPUNIT_ASSERT ( ! obj1->isChild(child) );
  CPPUNIT_ASSERT ( child->getParent() != obj1 );
  CPPUNIT_ASSERT ( obj2->hasChildren() );
  CPPUNIT_ASSERT ( obj2->numOfChildren() == 1 );
  CPPUNIT_ASSERT ( obj2->isChild(child) );
  CPPUNIT_ASSERT ( child->getParent() == obj2 );

  // Are the maximum number of child objects reached?
  CPPUNIT_ASSERT ( obj2->getMaxChildren() == finalcut::FObject::UNLIMITED );
  obj2->setMaxChildren(1);
  CPPUNIT_ASSERT ( obj2->hasChildren() );
  CPPUNIT_ASSERT ( obj2->getMaxChildren() == 1 );
  CPPUNIT_ASSERT ( obj2->numOfChildren() == 1 );
  auto child2 = new finalcut::FObject();
  CPPUNIT_ASSERT ( ! child2->hasParent() );
  CPPUNIT_ASSERT_THROW ( obj2->addChild(child2), std::length_error );
  CPPUNIT_ASSERT ( obj2->numOfChildren() == 1 );
  obj2->setMaxChildren(2);
  CPPUNIT_ASSERT ( ! child2->hasParent() );
  CPPUNIT_ASSERT ( obj2->getMaxChildren() == 2 );
  obj2->addChild(child2);
  CPPUNIT_ASSERT ( child2->hasParent() );
  CPPUNIT_ASSERT ( obj2->hasChildren() );
  CPPUNIT_ASSERT ( obj2->numOfChildren() == 2 );

  delete obj1;
  delete obj2;  // also deletes the child object
}

//----------------------------------------------------------------------
void FObjectTest::delTest()
{
  // obj -> child

  auto obj =  new finalcut::FObject();
  auto child = new finalcut::FObject(obj);

  CPPUNIT_ASSERT ( obj->hasChildren() );
  CPPUNIT_ASSERT ( obj->numOfChildren() == 1 );
  CPPUNIT_ASSERT ( obj->isChild(child) );

  CPPUNIT_ASSERT ( child->hasParent() );
  CPPUNIT_ASSERT ( child->getParent() == obj );

  obj->delChild(child);
  CPPUNIT_ASSERT ( ! obj->hasChildren() );
  CPPUNIT_ASSERT ( obj->numOfChildren() == 0 );
  CPPUNIT_ASSERT ( ! obj->isChild(child) );

  CPPUNIT_ASSERT ( ! child->hasParent() );
  CPPUNIT_ASSERT ( child->getParent() != obj );

  delete child;
  delete obj;
}

//----------------------------------------------------------------------
void FObjectTest::elementAccessTest()
{
  // obj -> child1
  //     -> child2
  //     -> child3
  //     -> child4
  //     -> child5

  auto obj =  new finalcut::FObject();
  auto child1 = new finalcut::FObject(obj);
  auto child2 = new finalcut::FObject(obj);
  auto child3 = new finalcut::FObject(obj);
  auto child4 = new finalcut::FObject(obj);
  auto child5 = new finalcut::FObject(obj);

  CPPUNIT_ASSERT ( child1->getParent() == obj );
  CPPUNIT_ASSERT ( child2->getParent() == obj );
  CPPUNIT_ASSERT ( child3->getParent() == obj );
  CPPUNIT_ASSERT ( child4->getParent() == obj );
  CPPUNIT_ASSERT ( child5->getParent() == obj );

  finalcut::FObject::const_reference c_first = obj->front();
  finalcut::FObject::const_reference c_last = obj->back();
  CPPUNIT_ASSERT ( c_first == child1 );
  CPPUNIT_ASSERT ( c_last == child5 );
  CPPUNIT_ASSERT ( obj->numOfChildren() == 5 );
  obj->delChild(child1);
  CPPUNIT_ASSERT ( obj->numOfChildren() == 4 );
  CPPUNIT_ASSERT ( obj->front() == child2 );
  CPPUNIT_ASSERT ( obj->back() == child5 );

  finalcut::FObject::reference first = obj->front();
  finalcut::FObject::reference last = obj->back();
  CPPUNIT_ASSERT ( first == child2 );
  CPPUNIT_ASSERT ( last == child5 );
  CPPUNIT_ASSERT ( obj->numOfChildren() == 4 );
  obj->delChild(child5);
  CPPUNIT_ASSERT ( obj->numOfChildren() == 3 );
  CPPUNIT_ASSERT ( obj->front() == child2 );
  CPPUNIT_ASSERT ( obj->back() == child4 );

  delete obj;
}

//----------------------------------------------------------------------
void FObjectTest::iteratorTest()
{
  // obj -> child1
  //     -> child2
  //     -> child3

  auto obj =  new finalcut::FObject();
  auto child1 = new finalcut::FObject(obj);
  auto child2 = new finalcut::FObject(obj);
  auto child3 = new finalcut::FObject(obj);

  CPPUNIT_ASSERT ( child1->getParent() == obj );
  CPPUNIT_ASSERT ( child2->getParent() == obj );
  CPPUNIT_ASSERT ( child3->getParent() == obj );

  finalcut::FObject::const_iterator c_iter, c_last;
  c_iter = obj->begin();
  c_last = obj->end();
  std::size_t i = 0;

  while ( c_iter != c_last )
  {
    i++;
    ++c_iter;
  }

  CPPUNIT_ASSERT ( obj->numOfChildren() == i );
  CPPUNIT_ASSERT ( i == 3 );

  finalcut::FObject::iterator iter, last;
  iter = obj->begin();
  last = obj->end();
  i = 0;

  while ( iter != last )
  {
    i++;
    ++iter;
  }

  CPPUNIT_ASSERT ( obj->numOfChildren() == i );
  CPPUNIT_ASSERT ( i == 3 );

  delete obj;
}

//----------------------------------------------------------------------
void FObjectTest::timeTest()
{
  struct timeval time1;
  uInt64 timeout = 750000;  // 750 ms
  finalcut::FObject::getCurrentTime(&time1);
  CPPUNIT_ASSERT ( ! finalcut::FObject::isTimeout (&time1, timeout) );
  sleep(1);
  CPPUNIT_ASSERT ( finalcut::FObject::isTimeout (&time1, timeout) );
  time1.tv_sec = 300;
  time1.tv_usec = 2000000;  // > 1000000 µs to test diff underflow
  CPPUNIT_ASSERT ( finalcut::FObject::isTimeout (&time1, timeout) );
}

//----------------------------------------------------------------------
void FObjectTest::timerTest()
{
  using finalcut::operator +;
  using finalcut::operator -;
  using finalcut::operator +=;
  using finalcut::operator <;

  test::FObject_protected t1;
  test::FObject_protected t2;
  int id1, id2;
  CPPUNIT_ASSERT ( t1.getTimerList()->empty() );
  id1 = t1.addTimer(300);
  CPPUNIT_ASSERT ( ! t1.getTimerList()->empty() );
  CPPUNIT_ASSERT ( t1.getTimerList()->size() == 1 );
  id2 = t1.addTimer(900);
  CPPUNIT_ASSERT ( t1.getTimerList()->size() == 2 );
  CPPUNIT_ASSERT ( &t1 != &t2 );
  CPPUNIT_ASSERT ( id1 != id2 );
  t1.delTimer (id1);
  CPPUNIT_ASSERT ( t1.getTimerList()->size() == 1 );
  t1.delTimer (id2);
  CPPUNIT_ASSERT ( t1.getTimerList()->empty() );
  CPPUNIT_ASSERT ( t1.getTimerList()->size() == 0 );

  id1 = t1.addTimer(45);
  id2 = t1.addTimer(95);
  t1.delTimer (id2);
  CPPUNIT_ASSERT ( t1.getTimerList()->size() == 1 );
  t1.delTimer (id1);
  CPPUNIT_ASSERT ( t1.getTimerList()->empty() );
  CPPUNIT_ASSERT ( t1.getTimerList()->size() == 0 );

  CPPUNIT_ASSERT ( ! t1.delTimer (id1) );  // id double delete
  CPPUNIT_ASSERT ( ! t1.delAllTimers() );

  t1.addTimer(250);
  t1.addTimer(500);
  t2.addTimer(750);
  t2.addTimer(1000);
  CPPUNIT_ASSERT_EQUAL ( t1.getTimerList(), t2.getTimerList() );
  CPPUNIT_ASSERT ( t1.getTimerList()->size() == 4 );
  CPPUNIT_ASSERT ( t2.getTimerList()->size() == 4 );

  t1.delOwnTimers();
  CPPUNIT_ASSERT ( t1.getTimerList()->size() == 2 );
  CPPUNIT_ASSERT ( t2.getTimerList()->size() == 2 );

  t1.addTimer(250);
  CPPUNIT_ASSERT ( t1.getTimerList()->size() == 3 );
  CPPUNIT_ASSERT ( t2.getTimerList()->size() == 3 );

  t2.delAllTimers();
  CPPUNIT_ASSERT ( t1.getTimerList()->empty() );
  CPPUNIT_ASSERT ( t2.getTimerList()->empty() );
  CPPUNIT_ASSERT ( t1.getTimerList()->size() == 0 );
  CPPUNIT_ASSERT ( t2.getTimerList()->size() == 0 );

  timeval tv1 = { 1321006271, 0 };
  timeval tv2 = { 27166271, 0 };
  timeval tv_sum = tv1 + tv2;
  CPPUNIT_ASSERT ( tv_sum.tv_sec == 1348172542 );
  CPPUNIT_ASSERT ( tv_sum.tv_usec == 0 );

  timeval tv_difference = tv1 - tv2;
  CPPUNIT_ASSERT ( tv_difference.tv_sec == 1293840000 );
  CPPUNIT_ASSERT ( tv_difference.tv_usec == 0 );

  tv_sum += tv2;
  CPPUNIT_ASSERT ( tv_sum.tv_sec == 1375338813 );
  CPPUNIT_ASSERT ( tv_sum.tv_usec == 0 );

  CPPUNIT_ASSERT ( tv2 < tv1 );
  CPPUNIT_ASSERT ( ! (tv1 < tv2) );
  CPPUNIT_ASSERT ( tv1 < tv_sum );
  CPPUNIT_ASSERT ( ! (tv_sum < tv1) );
  CPPUNIT_ASSERT ( tv2 < tv_sum );
  CPPUNIT_ASSERT ( ! (tv_sum < tv2) );
  CPPUNIT_ASSERT ( tv_difference < tv_sum );
  CPPUNIT_ASSERT ( ! (tv_sum < tv_difference) );

  tv1.tv_usec = tv2.tv_usec = 600000;
  tv_sum = tv1 + tv2;
  CPPUNIT_ASSERT ( tv_sum.tv_sec == 1348172543 );
  CPPUNIT_ASSERT ( tv_sum.tv_usec == 200000 );

  tv1.tv_usec = 654321;
  tv2.tv_usec = 123456;
  tv_difference = tv1 - tv2;
  CPPUNIT_ASSERT ( tv_difference.tv_sec == 1293840000 );
  CPPUNIT_ASSERT ( tv_difference.tv_usec == 530865 );

  tv2.tv_usec = 999888;
  tv_sum += tv2;
  CPPUNIT_ASSERT ( tv_sum.tv_sec == 1375338815 );
  CPPUNIT_ASSERT ( tv_sum.tv_usec == 199888 );

  CPPUNIT_ASSERT ( tv2 < tv1 );
  CPPUNIT_ASSERT ( ! (tv1 < tv2) );
  CPPUNIT_ASSERT ( tv_difference < tv_sum );
  CPPUNIT_ASSERT ( ! (tv_sum < tv_difference) );

  CPPUNIT_ASSERT ( ! t1.delTimer(0) );
  CPPUNIT_ASSERT ( ! t1.delTimer(-1) );
}

//----------------------------------------------------------------------
void FObjectTest::performTimerActionTest()
{
  test::FObject_protected t1;
  uInt num_events = 0;
  uInt loop = 0;
  t1.addTimer(100);

  while ( loop < 10 )
  {
    num_events += t1.processEvent();
    // Wait 100 ms
    const struct timespec ms[]{{0, 100000000L}};
    nanosleep (ms, NULL);
    loop++;
  }

  CPPUNIT_ASSERT ( loop == 10 );
  CPPUNIT_ASSERT ( num_events == 9 );
  CPPUNIT_ASSERT ( t1.count == 9 );

  test::FObject_timer t2;
  CPPUNIT_ASSERT ( t2.getValue() == 0 );
  finalcut::FTimerEvent timer_ev (finalcut::Event::Timer, 1);

  for (auto x = 0; x < 10; x++)
    finalcut::FApplication::sendEvent (&t2, &timer_ev);

  CPPUNIT_ASSERT ( t2.getValue() == 10 );
}

//----------------------------------------------------------------------
void FObjectTest::userEventTest()
{
  test::FObject_userEvent user;
  CPPUNIT_ASSERT ( user.getValue() == 0 );

  int n = 9;
  finalcut::FUserEvent user_ev (finalcut::Event::User, 42);
  user_ev.setData(n);
  finalcut::FApplication::sendEvent (&user, &user_ev);
  CPPUNIT_ASSERT ( user.getValue() == 9 );
  CPPUNIT_ASSERT ( n == 10 );
}

// Put the test suite in the registry
CPPUNIT_TEST_SUITE_REGISTRATION (FObjectTest);

// The general unit test main part
#include <main-test.inc>
