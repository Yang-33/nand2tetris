#include <gtest/gtest.h>
#include "sstring.h"

class TestTest123 : public ::testing::Test
{
public:
	virtual void SetUp()
	{
		m_pString = new String();
	}

	virtual void TearDown()
	{
		delete m_pString;
		m_pString = NULL;
	}

public:
	String* m_pString;
};


TEST_F(TestTest123, Add1)
{
	EXPECT_EQ(m_pString->Add("aaa", "bbb"), "aaabbb");
}

TEST_F(TestTest123, Add2)
{
	EXPECT_EQ(m_pString->Add("1", "2"), "12");
}

TEST_F(TestTest123, elsetest)
{
	int b = 10;
	EXPECT_EQ(b, 10);
}