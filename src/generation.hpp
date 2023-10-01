#pragma once

#include <sstream>
#include <unordered_map>
#include "./parser.hpp"


class Generator 
{
    public:
        inline Generator(NodeProg prog) 
            : m_prog(std::move(prog))
        {
        }

        void gen_expr(const NodeExpr& expr) {
            struct ExprVisitor {
                Generator* gen;
                void operator()(const NodeExprIntLit& expr_int_lit) {
                    gen->m_output << "    mov rax, " << expr_int_lit.int_lit.value.value() << "\n";
                    gen->push("rax");
                }
                void operator()(const NodeExprIdent& expr_ident) {
                    std::string identifier = expr_ident.ident.value.value();
                    if (gen->m_vars.count(identifier) <= 0) {
                        std::cerr << "Undeclared identifier: " << identifier << std::endl;
                        exit(EXIT_FAILURE);
                    }
                    const auto& var = gen->m_vars.at(identifier);
                    std::stringstream offset;
                    offset << "QWORD [rsp + " << (gen->m_stack_size - var.stack_location - 1) * 8 << "]\n";
                    gen->push(offset.str());
                }
            };
            ExprVisitor visitor({.gen = this});
            std::visit(visitor, expr.var);
        }

        void gen_stmt(const NodeStmt& stmt) {
            struct StmtVisitor {
                Generator* gen;
                void operator()(const NodeStmtExit& stmt_exit) {
                    gen->gen_expr(stmt_exit.expr);
                    gen->m_output << "    mov rax, 60\n";
                    gen->pop("rdi");
                    gen->m_output << "    syscall" << '\n';
                }
                void operator()(const NodeStmtLet& stmt_let) {
                    std::string identifier = stmt_let.ident.value.value();
                    // std::cout << identifier << " " << gen->m_vars.count(identifier) << std::endl;
                    if (gen->m_vars.count(identifier) > 0) {
                        std::cerr << "Identifier already used " << identifier << std::endl;
                        exit(EXIT_FAILURE);
                    }
                    gen->m_vars.insert({identifier, Var{.stack_location = gen->m_stack_size}});
                    gen->gen_expr(stmt_let.expr);
                }
            };
            StmtVisitor visitor({.gen = this});
            std::visit(visitor, stmt.var);
        }

        [[nodiscard]] std::string gen_prog() {
            // std::stringstream output;
            m_output << "global _start\n_start:\n";
            
            for (const NodeStmt& stmt : m_prog.stmts) {
                gen_stmt(stmt);
            }
            m_output << "    mov rax, 60\n";
            m_output << "    mov rdi, 0\n";
            m_output << "    syscall" << '\n';
            return m_output.str();
        }
    private:

        void push(const std::string& reg) {
            m_output << "    push " << reg << "\n"; 
            m_stack_size++;
        }

        void pop(const std::string& reg) {
            m_output << "    pop " << reg << "\n";
            m_stack_size--;
        }

        struct Var {
            size_t stack_location;
        };

        const NodeProg m_prog;
        std::stringstream m_output;
        size_t m_stack_size = 0;
        std::unordered_map<std::string, Var> m_vars {};
};  