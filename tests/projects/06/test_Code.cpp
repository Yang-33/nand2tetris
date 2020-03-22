#include <gtest/gtest.h>
#include "projects/06/Code.h"


TEST(test_Code_comp, LookUpComp)
{
    nand2tetris::Code m_pCode;
    EXPECT_EQ(m_pCode.comp("0"), 0b0101010);
    EXPECT_EQ(m_pCode.comp("1"), 0b0111111);
    EXPECT_EQ(m_pCode.comp("-1"), 0b0111010);
    EXPECT_EQ(m_pCode.comp("D"), 0b0001100);
    EXPECT_EQ(m_pCode.comp("A"), 0b0110000);

    EXPECT_EQ(m_pCode.comp("!D"), 0b0001101);
    EXPECT_EQ(m_pCode.comp("!A"), 0b0110001);
    EXPECT_EQ(m_pCode.comp("-D"), 0b0001111);
    EXPECT_EQ(m_pCode.comp("-A"), 0b0110011);
    EXPECT_EQ(m_pCode.comp("D+1"), 0b0011111);

    EXPECT_EQ(m_pCode.comp("A+1"), 0b0110111);
    EXPECT_EQ(m_pCode.comp("D-1"), 0b0001110);
    EXPECT_EQ(m_pCode.comp("A-1"), 0b0110010);
    EXPECT_EQ(m_pCode.comp("D+A"), 0b0000010);
    EXPECT_EQ(m_pCode.comp("D-A"), 0b0010011);

    EXPECT_EQ(m_pCode.comp("A-D"), 0b0000111);
    EXPECT_EQ(m_pCode.comp("D&A"), 0b0000000);
    EXPECT_EQ(m_pCode.comp("D|A"), 0b0010101);

    EXPECT_EQ(m_pCode.comp("M"), 0b1110000);
    EXPECT_EQ(m_pCode.comp("!M"), 0b1110001);
    EXPECT_EQ(m_pCode.comp("-M"), 0b1110011);
    EXPECT_EQ(m_pCode.comp("M+1"), 0b1110111);
    EXPECT_EQ(m_pCode.comp("M-1"), 0b1110010);

    EXPECT_EQ(m_pCode.comp("D+M"), 0b1000010);
    EXPECT_EQ(m_pCode.comp("D-M"), 0b1010011);
    EXPECT_EQ(m_pCode.comp("M-D"), 0b1000111);
    EXPECT_EQ(m_pCode.comp("D&M"), 0b1000000);
    EXPECT_EQ(m_pCode.comp("D|M"), 0b1010101);

}

TEST(test_Code_dest, LookUpDest)
{
    nand2tetris::Code m_pCode;
    EXPECT_EQ(m_pCode.dest(""), 0b000);
    EXPECT_EQ(m_pCode.dest("M"), 0b001);
    EXPECT_EQ(m_pCode.dest("D"), 0b010);
    EXPECT_EQ(m_pCode.dest("MD"), 0b011);
    EXPECT_EQ(m_pCode.dest("A"), 0b100);
    EXPECT_EQ(m_pCode.dest("AM"), 0b101);
    EXPECT_EQ(m_pCode.dest("AD"), 0b110);
    EXPECT_EQ(m_pCode.dest("AMD"), 0b111);
}

TEST(test_Code_jump, LookUpJump)
{
    nand2tetris::Code m_pCode;
    EXPECT_EQ(m_pCode.jump(""), 0b000);
    EXPECT_EQ(m_pCode.jump("JGT"), 0b001);
    EXPECT_EQ(m_pCode.jump("JEQ"), 0b010);
    EXPECT_EQ(m_pCode.jump("JGE"), 0b011);
    EXPECT_EQ(m_pCode.jump("JLT"), 0b100);
    EXPECT_EQ(m_pCode.jump("JNE"), 0b101);
    EXPECT_EQ(m_pCode.jump("JLE"), 0b110);
    EXPECT_EQ(m_pCode.jump("JMP"), 0b111);
}

TEST(test_Code_ACode, ACode)
{
    nand2tetris::Code m_pCode;
    EXPECT_EQ(m_pCode.ACode(0), "0000000000000000");
    EXPECT_EQ(m_pCode.ACode(0b1101), "0000000000001101");
}

TEST(test_Code_CCode, CCode)
{
    nand2tetris::Code m_pCode;
    EXPECT_EQ(m_pCode.CCode("D|A", "AM", ""), "1110010101101000");
    EXPECT_EQ(m_pCode.CCode("M-1", "M", "JNE"), "1111110010001101");
}