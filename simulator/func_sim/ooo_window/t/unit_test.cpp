
#include <catch.hpp>

#include <func_sim/ooo_window/ooo_window.h>
#include <risc_v/riscv_instr.h>

TEST_CASE( "OOO_Window: zero size")
{
    OOOWindow<RISCVInstr<uint32>> w( 0);
    RISCVInstr<uint32> nop(0x0001);
    w.write_instruction( nop);
    w.write_instruction( nop);
    w.write_instruction( nop);

    CHECK( w.get_size() == 0);
    CHECK( w.get_instr_count() == 0);
    CHECK( !w.is_full());
    CHECK( w.count_dependent_instructions() == 0);
    CHECK( w.count_independent_instructions() == 0);
    CHECK( w.get_avg_independent_instructions() == 0);
}

TEST_CASE( "OOO_Window: all independent")
{
    OOOWindow<RISCVInstr<uint32>> w( 5);
    RISCVInstr<uint32> instr(0x48fd); // c_li $a7, 31

    for ( int i = 0; i < 1000; i++)
        w.write_instruction( instr);

    CHECK( w.get_size() == 5);
    CHECK( w.get_instr_count() == 5);
    CHECK( w.is_full());
    CHECK( w.count_dependent_instructions() == 0);
    CHECK( w.count_independent_instructions() == 5);
    CHECK( w.get_avg_independent_instructions() == 5.0);
}

TEST_CASE( "OOO_Window: all dependent")
{
    OOOWindow<RISCVInstr<uint32>> w( 10);
    RISCVInstr<uint32> instr(0x8df1); // c_and $a1, $a2

    for ( int i = 0; i < 1000; i++)
        w.write_instruction( instr);

    CHECK( w.get_size() == 10);
    CHECK( w.get_instr_count() == 10);
    CHECK( w.is_full());
    CHECK( w.count_dependent_instructions() == 9); // the last instr. in the window is always independent
    CHECK( w.count_independent_instructions() == 1);
    CHECK( w.get_avg_independent_instructions() == 1.0);
}