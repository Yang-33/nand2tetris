#include <gtest/gtest.h>
#include <vector>
#include "nest/svector.h"

class TestTestVec : public ::testing::Test
{
public:
	virtual void SetUp()
	{
		m_pVector = new Vector();
	}

	virtual void TearDown()
	{
		delete m_pVector;
		m_pVector = NULL;
	}

public:
	Vector* m_pVector;
};


TEST_F(TestTestVec, Add1)
{
	std::vector<int> a = { 1 };
	std::vector<int> b = { 2 };
	std::vector<int> c = { 1,2 };
	EXPECT_EQ(m_pVector->Add(a, b), c);
}

TEST_F(TestTestVec, Add2)
{
	std::vector<int> a = { 1,2 };
	std::vector<int> b = { 3 };
	std::vector<int> c = { 1,2,3 };
	EXPECT_EQ(m_pVector->Add(a, b), c);
}
