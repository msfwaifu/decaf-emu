#pragma once
#include <string>
#include <spdlog/spdlog.h>
#include "gpu/latte.h"
#include "gpu/latte_shadir.h"

struct GenerateState
{
   fmt::MemoryWriter out;
   std::string indent;
   int32_t cfPC = -1;
   int32_t groupPC = -1;
   latte::Shader *shader = nullptr;
};

using TranslateFuncCF = bool(*)(GenerateState &state, latte::shadir::CfInstruction *ins);
using TranslateFuncALU = bool(*)(GenerateState &state, latte::shadir::AluInstruction *ins);
using TranslateFuncALUReduction = bool(*)(GenerateState &state, latte::shadir::AluReductionInstruction *ins);
using TranslateFuncTEX = bool(*)(GenerateState &state, latte::shadir::TexInstruction *ins);
using TranslateFuncEXP = bool(*)(GenerateState &state, latte::shadir::ExportInstruction *ins);

namespace gpu
{

namespace opengl
{

namespace glsl
{

void intialise();

bool generateBody(latte::Shader &shader, std::string &body);

void registerGenerator(latte::cf::inst ins, TranslateFuncCF func);
void registerGenerator(latte::alu::op2 ins, TranslateFuncALU func);
void registerGenerator(latte::alu::op3 ins, TranslateFuncALU func);
void registerGenerator(latte::alu::op2 ins, TranslateFuncALUReduction func);
void registerGenerator(latte::tex::inst ins, TranslateFuncTEX func);
void registerGenerator(latte::exp::inst ins, TranslateFuncEXP func);

void registerCf();
void registerTex();
void registerExp();
void registerAluOP2();
void registerAluOP3();
void registerAluReduction();

void beginLine(GenerateState &state);
void endLine(GenerateState &state);
void increaseIndent(GenerateState &state);
void decreaseIndent(GenerateState &state);

using latte::shadir::AluSource;
using latte::shadir::AluInstruction;
using latte::shadir::SelRegister;
using latte::alu::Channel::Channel;

void translateAluDestStart(GenerateState &state, AluInstruction *ins);
void translateAluDestEnd(GenerateState &state, AluInstruction *ins);
void translateAluSource(GenerateState &state, const AluSource &src);
void translateAluSourceVector(GenerateState &state, const AluSource &srcX, const AluSource &srcY, const AluSource &srcZ, const AluSource &srcW);

void translateChannel(GenerateState &state, Channel channel);
unsigned translateSelRegister(GenerateState &state, const SelRegister &reg, size_t maxSel = 4);

} // namespace glsl

} // namespace opengl

} // namespace gpu
