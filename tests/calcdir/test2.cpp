#include <gtest/gtest.h>
#include "calcdir/calc.h"

class TestTest : public ::testing::Test
{
public:
	virtual void SetUp()
	{
		m_pCalc = new Calc();
	}

	virtual void TearDown()
	{
		delete m_pCalc;
		m_pCalc = NULL;
	}

public:
	Calc* m_pCalc;
};


TEST_F(TestTest, Add)
{
	EXPECT_EQ(m_pCalc->Add(1, 2), 3);
}

TEST_F(TestTest, Subtract)
{
	EXPECT_EQ(m_pCalc->Subtract(2, 1), 1);
}

TEST_F(TestTest, Divide)
{
	EXPECT_EQ(m_pCalc->Divide(2, 1), 2);
}

TEST_F(TestTest, DivideByZero)
{
	ASSERT_THROW(m_pCalc->Divide(2, 0), DivideByZeroException);
}
