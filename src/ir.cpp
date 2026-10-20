#include "ir.hpp"
#include <sstream>
#include <utility>
namespace minic { namespace {
class Generator { public: std::vector<IRInstruction> run(const std::vector<Assignment>& p){for(const auto& a:p){auto v=expr(*a.value);ir_.push_back({IROp::Store,a.name,v,"",0,a.loc});}return std::move(ir_);} private:
std::string temp(){return "t"+std::to_string(++next_);} std::string expr(const Expr& e){if(auto n=dynamic_cast<const NumberExpr*>(&e)){auto d=temp();ir_.push_back({IROp::LoadConst,d,"","",n->value,n->loc});return d;}if(auto v=dynamic_cast<const VariableExpr*>(&e)){auto d=temp();ir_.push_back({IROp::LoadVar,d,v->name,"",0,v->loc});return d;}if(auto u=dynamic_cast<const UnaryExpr*>(&e)){auto a=expr(*u->operand);auto d=temp();ir_.push_back({IROp::Neg,d,a,"",0,u->loc});return d;}auto b=dynamic_cast<const BinaryExpr*>(&e);auto l=expr(*b->left);auto r=expr(*b->right);auto d=temp();IROp op=b->op=='+'?IROp::Add:b->op=='-'?IROp::Sub:b->op=='*'?IROp::Mul:IROp::Div;ir_.push_back({op,d,l,r,0,b->loc});return d;}
std::vector<IRInstruction> ir_;int next_=0;};}
std::vector<IRInstruction> generateIR(const std::vector<Assignment>& p){return Generator().run(p);}
const char* irOpName(IROp o){switch(o){case IROp::LoadConst:return "const";case IROp::LoadVar:return "load";case IROp::Add:return "+";case IROp::Sub:return "-";case IROp::Mul:return "*";case IROp::Div:return "/";case IROp::Neg:return "neg";case IROp::Store:return "store";}return "?";}
std::string irToString(const std::vector<IRInstruction>& ir){std::ostringstream o;for(const auto& i:ir){switch(i.op){case IROp::LoadConst:o<<i.dest<<" = "<<i.value;break;case IROp::LoadVar:o<<i.dest<<" = "<<i.left;break;case IROp::Add:case IROp::Sub:case IROp::Mul:case IROp::Div:o<<i.dest<<" = "<<i.left<<" "<<irOpName(i.op)<<" "<<i.right;break;case IROp::Neg:o<<i.dest<<" = -"<<i.left;break;case IROp::Store:o<<i.dest<<" = "<<i.left;break;}o<<'\n';}return o.str();}
}
