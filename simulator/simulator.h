/*
 * simulator.h - interface for simulator
 * Copyright 2018 MIPT-MIPS
 */

#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <infra/log.h>
#include <infra/target.h>
#include <infra/types.h>
 
#include <memory>

class Simulator : public Log {
public:
    explicit Simulator( bool log = false) : Log( log) {}

    virtual void run( const std::string& tr, uint64 instrs_to_run) = 0;
    void run_no_limit( const std::string& tr) { run( tr, MAX_VAL64); }
    virtual void set_target( const Target& target) = 0;

    static std::unique_ptr<Simulator> create_simulator( const std::string& isa, bool functional_only, bool log);
    static std::unique_ptr<Simulator> create_configured_simulator();

    /* GDB interfaces */
    virtual void gdb_load( const std::string &tr) = 0;
    virtual void gdb_prepare() = 0;
    virtual void gdb_resume( int steps) = 0;
    virtual int gdb_mem_read( unsigned int addr, unsigned char *buf, int length) = 0;
    virtual int gdb_mem_write( unsigned int addr, const unsigned char *buf, int length) = 0;
};

#endif // SIMULATOR_H
