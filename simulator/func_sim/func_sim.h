/*
 * func_sim.cpp - extremely simple simulator
 * @author Pavel Kryukov pavel.kryukov@phystech.edu
 * Copyright 2015 MIPT-MIPS
 */

#ifndef FUNC_SIM_H
#define FUNC_SIM_H

#include "rf/rf.h"

#include <infra/exception.h>

#include <simulator.h>

#include <memory>
#include <string>

struct BearingLost final : Exception
{
    BearingLost() : Exception("Bearing lost", "10 nops in a row") { }
};

template <typename ISA>
class FuncSim : public Simulator
{
    using FuncInstr = typename ISA::FuncInstr;
    using Memory = typename ISA::Memory;
    
    private:
        RF<ISA> rf;
        Addr PC = NO_VAL32;
        uint64 sequence_id = 0;
        std::unique_ptr<Memory> mem = nullptr;

        uint64 nops_in_a_row = 0;
        void update_and_check_nop_counter( const FuncInstr& instr);

    public:
        explicit FuncSim( bool log = false);

        FuncInstr step();
        void init( const std::string& tr) final;
        void run( uint64 instrs_to_run) final;
        void set_target(const Target& target) final {
            PC = target.address;
            sequence_id = target.sequence_id;
        }

        void load_binary_file( const std::string &tr) final;
        void prepare_to_run() final;
        size_t mem_read( Addr addr, unsigned char *buf, size_t length) final;
        size_t mem_write( Addr addr, const unsigned char *buf, size_t length) final;
};

#endif
