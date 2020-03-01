#include <gtest/gtest.h>
#include "calc.h"

struct ValueResult
{
	int a;
	int b;
	int c;
};

class TestTestParam : public testing::TestWithParam<ValueResult>
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

typedef TestTestParam TestTestParamAdd;
typedef TestTestParam TestTestParamSubtract;
typedef TestTestParam TestTestParamDivide;

TEST_P(TestTestParamAdd, Add)
{
	ValueResult value = GetParam();
	EXPECT_EQ(m_pCalc->Add(value.a, value.b), value.c);
}

TEST_P(TestTestParamSubtract, Subtract)
{
	ValueResult value = GetParam();
	EXPECT_EQ(m_pCalc->Subtract(value.a, value.b), value.c);
}

TEST_P(TestTestParamDivide, Divide)
{
	ValueResult value = GetParam();
	EXPECT_EQ(m_pCalc->Divide(value.a, value.b), value.c);
}

const ValueResult valuesAdd[] =
{
	{0, 1, 1},
	{1, 2, 3},
};

INSTANTIATE_TEST_CASE_P(TestTestParamInstance,
						TestTestParamAdd,
						::testing::ValuesIn(valuesAdd));


const ValueResult valuesSubtract[] =
{
	{2, 1, 1},
	{3, 2, 1},
};

INSTANTIATE_TEST_CASE_P(TestTestParamInstance,
						TestTestParamSubtract,
						::testing::ValuesIn(valuesSubtract));


const ValueResult valuesDivide[] =
{
	{2, 1, 2},
	{3, 2, 1},
};

INSTANTIATE_TEST_CASE_P(TestTestParamInstance,
						TestTestParamDivide,
						::testing::ValuesIn(valuesDivide));


