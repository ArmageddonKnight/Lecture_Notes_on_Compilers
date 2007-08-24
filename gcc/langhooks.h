/* The lang_hooks data structure.
   Copyright 2001, 2002, 2003, 2004, 2005, 2006, 2007
   Free Software Foundation, Inc.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3, or (at your option)
any later version.

GCC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */

#ifndef GCC_LANG_HOOKS_H
#define GCC_LANG_HOOKS_H

/* This file should be #include-d after tree.h.  */

struct diagnostic_context;

struct gimplify_omp_ctx;

/* A print hook for print_tree ().  */
typedef void (*lang_print_tree_hook) (FILE *, tree, int indent);

/* The following hooks are documented in langhooks.c.  Must not be
   NULL.  */

struct lang_hooks_for_tree_inlining
{
  bool (*var_mod_type_p) (tree, tree);
};

struct lang_hooks_for_callgraph
{
  /* The node passed is a language-specific tree node.  If its contents
     are relevant to use of other declarations, mark them.  */
  tree (*analyze_expr) (tree *, int *, tree);

  /* Produce RTL for function passed as argument.  */
  void (*expand_function) (tree);
};

/* Lang hooks for management of language-specific data or status
   when entering / leaving functions etc.  */
struct lang_hooks_for_functions
{
  /* Called when entering a function.  */
  void (*init) (struct function *);

  /* Called when leaving a function.  */
  void (*final) (struct function *);

  /* Called when entering a nested function.  */
  void (*enter_nested) (struct function *);

  /* Called when leaving a nested function.  */
  void (*leave_nested) (struct function *);

  /* Determines if it's ok for a function to have no noreturn attribute.  */
  bool (*missing_noreturn_ok_p) (tree);
};

/* The following hooks are used by tree-dump.c.  */

struct lang_hooks_for_tree_dump
{
  /* Dump language-specific parts of tree nodes.  Returns nonzero if it
     does not want the usual dumping of the second argument.  */
  bool (*dump_tree) (void *, tree);

  /* Determine type qualifiers in a language-specific way.  */
  int (*type_quals) (const_tree);
};

/* Hooks related to types.  */

struct lang_hooks_for_types
{
  /* Return a new type (with the indicated CODE), doing whatever
     language-specific processing is required.  */
  tree (*make_type) (enum tree_code);

  /* Given MODE and UNSIGNEDP, return a suitable type-tree with that
     mode.  */
  tree (*type_for_mode) (enum machine_mode, int);

  /* Given PRECISION and UNSIGNEDP, return a suitable type-tree for an
     integer type with at least that precision.  */
  tree (*type_for_size) (unsigned, int);

  /* True if the type is an instantiation of a generic type,
     e.g. C++ template implicit specializations.  */
  bool (*generic_p) (const_tree);

  /* Given a type, apply default promotions to unnamed function
     arguments and return the new type.  Return the same type if no
     change.  Required by any language that supports variadic
     arguments.  The default hook dies.  */
  tree (*type_promotes_to) (tree);

  /* Register TYPE as a builtin type with the indicated NAME.  The
     TYPE is placed in the outermost lexical scope.  The semantics
     should be analogous to:

       typedef TYPE NAME;

     in C.  The default hook ignores the declaration.  */
  void (*register_builtin_type) (tree, const char *);

  /* This routine is called in tree.c to print an error message for
     invalid use of an incomplete type.  VALUE is the expression that
     was used (or 0 if that isn't known) and TYPE is the type that was
     invalid.  */
  void (*incomplete_type_error) (const_tree value, const_tree type);

  /* Called from assign_temp to return the maximum size, if there is one,
     for a type.  */
  tree (*max_size) (const_tree);

  /* Register language specific type size variables as potentially OpenMP
     firstprivate variables.  */
  void (*omp_firstprivatize_type_sizes) (struct gimplify_omp_ctx *, tree);

  /* Nonzero if types that are identical are to be hashed so that only
     one copy is kept.  If a language requires unique types for each
     user-specified type, such as Ada, this should be set to TRUE.  */
  bool hash_types;
};

/* Language hooks related to decls and the symbol table.  */

struct lang_hooks_for_decls
{
  /* Returns nonzero if we are in the global binding level.  Ada
     returns -1 for an undocumented reason used in stor-layout.c.  */
  int (*global_bindings_p) (void);

  /* Insert BLOCK at the end of the list of subblocks of the
     current binding level.  This is used when a BIND_EXPR is expanded,
     to handle the BLOCK node inside the BIND_EXPR.  */
  void (*insert_block) (tree);

  /* Function to add a decl to the current scope level.  Takes one
     argument, a decl to add.  Returns that decl, or, if the same
     symbol is already declared, may return a different decl for that
     name.  */
  tree (*pushdecl) (tree);

  /* Returns the chain of decls so far in the current scope level.  */
  tree (*getdecls) (void);

  /* Returns true when we should warn for an unused global DECL.
     We will already have checked that it has static binding.  */
  bool (*warn_unused_global) (const_tree);

  /* Obtain a list of globals and do final output on them at end
     of compilation */
  void (*final_write_globals) (void);

  /* True if this decl may be called via a sibcall.  */
  bool (*ok_for_sibcall) (tree);

  /* Return the COMDAT group into which this DECL should be placed.
     It is known that the DECL belongs in *some* COMDAT group when
     this hook is called.  The return value will be used immediately,
     but not explicitly deallocated, so implementations should not use
     xmalloc to allocate the string returned.  (Typically, the return
     value will be the string already stored in an
     IDENTIFIER_NODE.)  */
  const char * (*comdat_group) (tree);

  /* True if OpenMP should privatize what this DECL points to rather
     than the DECL itself.  */
  bool (*omp_privatize_by_reference) (tree);

  /* Return sharing kind if OpenMP sharing attribute of DECL is
     predetermined, OMP_CLAUSE_DEFAULT_UNSPECIFIED otherwise.  */
  enum omp_clause_default_kind (*omp_predetermined_sharing) (tree);

  /* Return true if DECL's DECL_VALUE_EXPR (if any) should be
     disregarded in OpenMP construct, because it is going to be
     remapped during OpenMP lowering.  SHARED is true if DECL
     is going to be shared, false if it is going to be privatized.  */
  bool (*omp_disregard_value_expr) (tree, bool);

  /* Return true if DECL that is shared iff SHARED is true should
     be put into OMP_CLAUSE_PRIVATE_DEBUG.  */
  bool (*omp_private_debug_clause) (tree, bool);

  /* Build and return code for a default constructor for DECL in
     response to CLAUSE.  Return NULL if nothing to be done.  */
  tree (*omp_clause_default_ctor) (tree clause, tree decl);

  /* Build and return code for a copy constructor from SRC to DST.  */
  tree (*omp_clause_copy_ctor) (tree clause, tree dst, tree src);

  /* Similarly, except use an assignment operator instead.  */
  tree (*omp_clause_assign_op) (tree clause, tree dst, tree src);

  /* Build and return code destructing DECL.  Return NULL if nothing
     to be done.  */
  tree (*omp_clause_dtor) (tree clause, tree decl);
};

/* Language-specific hooks.  See langhooks-def.h for defaults.  */

struct lang_hooks
{
  /* String identifying the front end.  e.g. "GNU C++".  */
  const char *name;

  /* sizeof (struct lang_identifier), so make_node () creates
     identifier nodes long enough for the language-specific slots.  */
  size_t identifier_size;

  /* Determines the size of any language-specific tcc_constant or
     tcc_exceptional nodes.  Since it is called from make_node, the
     only information available is the tree code.  Expected to die
     on unrecognized codes.  */
  size_t (*tree_size) (enum tree_code);

  /* The first callback made to the front end, for simple
     initialization needed before any calls to handle_option.  Return
     the language mask to filter the switch array with.  */
  unsigned int (*init_options) (unsigned int argc, const char **argv);

  /* Callback used to perform language-specific initialization for the
     global diagnostic context structure.  */
  void (*initialize_diagnostics) (struct diagnostic_context *);

  /* Handle the switch CODE, which has real type enum opt_code from
     options.h.  If the switch takes an argument, it is passed in ARG
     which points to permanent storage.  The handler is responsible for
     checking whether ARG is NULL, which indicates that no argument
     was in fact supplied.  For -f and -W switches, VALUE is 1 or 0
     for the positive and negative forms respectively.

     Return 1 if the switch is valid, 0 if invalid, and -1 if it's
     valid and should not be treated as language-independent too.  */
  int (*handle_option) (size_t code, const char *arg, int value);

  /* Return false to use the default complaint about a missing
     argument, otherwise output a complaint and return true.  */
  bool (*missing_argument) (const char *opt, size_t code);

  /* Called when all command line options have been parsed to allow
     further processing and initialization

     Should return true to indicate that a compiler back-end is
     not required, such as with the -E option.

     If errorcount is nonzero after this call the compiler exits
     immediately and the finish hook is not called.  */
  bool (*post_options) (const char **);

  /* Called after post_options to initialize the front end.  Return
     false to indicate that no further compilation be performed, in
     which case the finish hook is called immediately.  */
  bool (*init) (void);

  /* Called at the end of compilation, as a finalizer.  */
  void (*finish) (void);

  /* Parses the entire file.  The argument is nonzero to cause bison
     parsers to dump debugging information during parsing.  */
  void (*parse_file) (int);

  /* Called immediately after parsing to clear the binding stack.  */
  void (*clear_binding_stack) (void);

  /* Called to obtain the alias set to be used for an expression or type.
     Returns -1 if the language does nothing special for it.  */
  alias_set_type (*get_alias_set) (tree);

  /* Called with an expression that is to be processed as a constant.
     Returns either the same expression or a language-independent
     constant equivalent to its input.  */
  tree (*expand_constant) (tree);

  /* Called by expand_expr for language-specific tree codes.
     Fourth argument is actually an enum expand_modifier.  */
  rtx (*expand_expr) (tree, rtx, enum machine_mode, int, rtx *);

  /* Called by expand_expr to generate the definition of a decl.  Returns
     1 if handled, 0 otherwise.  */
  int (*expand_decl) (tree);

  /* Function to finish handling an incomplete decl at the end of
     compilation.  Default hook is does nothing.  */
  void (*finish_incomplete_decl) (tree);

  /* Mark EXP saying that we need to be able to take the address of
     it; it should not be allocated in a register.  Return true if
     successful.  */
  bool (*mark_addressable) (tree);

  /* Hook called by staticp for language-specific tree codes.  */
  tree (*staticp) (tree);

  /* Replace the DECL_LANG_SPECIFIC data, which may be NULL, of the
     DECL_NODE with a newly GC-allocated copy.  */
  void (*dup_lang_specific_decl) (tree);

  /* Set the DECL_ASSEMBLER_NAME for a node.  If it is the sort of
     thing that the assembler should talk about, set
     DECL_ASSEMBLER_NAME to an appropriate IDENTIFIER_NODE.
     Otherwise, set it to the ERROR_MARK_NODE to ensure that the
     assembler does not talk about it.  */
  void (*set_decl_assembler_name) (tree);

  /* Nonzero if operations on types narrower than their mode should
     have their results reduced to the precision of the type.  */
  bool reduce_bit_field_operations;

  /* Nonzero if this front end does not generate a dummy BLOCK between
     the outermost scope of the function and the FUNCTION_DECL.  See
     is_body_block in stmt.c, and its callers.  */
  bool no_body_blocks;

  /* The front end can add its own statistics to -fmem-report with
     this hook.  It should output to stderr.  */
  void (*print_statistics) (void);

  /* Called by print_tree when there is a tree of class tcc_exceptional
     that it doesn't know how to display.  */
  lang_print_tree_hook print_xnode;

  /* Called to print language-dependent parts of tcc_decl, tcc_type,
     and IDENTIFIER_NODE nodes.  */
  lang_print_tree_hook print_decl;
  lang_print_tree_hook print_type;
  lang_print_tree_hook print_identifier;

  /* Computes the name to use to print a declaration.  DECL is the
     non-NULL declaration in question.  VERBOSITY determines what
     information will be printed: 0: DECL_NAME, demangled as
     necessary.  1: and scope information.  2: and any other
     information that might be interesting, such as function parameter
     types in C++.  */
  const char *(*decl_printable_name) (tree decl, int verbosity);

  /* Computes the dwarf-2/3 name for a tree.  VERBOSITY determines what
     information will be printed: 0: DECL_NAME, demangled as
     necessary.  1: and scope information.  */
  const char *(*dwarf_name) (tree, int verbosity);

  /* This compares two types for equivalence ("compatible" in C-based languages).
     This routine should only return 1 if it is sure.  It should not be used
     in contexts where erroneously returning 0 causes problems.  */
  int (*types_compatible_p) (tree x, tree y);

  /* Given a CALL_EXPR, return a function decl that is its target.  */
  tree (*lang_get_callee_fndecl) (const_tree);

  /* Called by report_error_function to print out function name.  */
  void (*print_error_function) (struct diagnostic_context *, const char *);

  /* Called from expr_size to calculate the size of the value of an
     expression in a language-dependent way.  Returns a tree for the size
     in bytes.  A frontend can call lhd_expr_size to get the default
     semantics in cases that it doesn't want to handle specially.  */
  tree (*expr_size) (const_tree);

  /* Convert a character from the host's to the target's character
     set.  The character should be in what C calls the "basic source
     character set" (roughly, the set of characters defined by plain
     old ASCII).  The default is to return the character unchanged,
     which is correct in most circumstances.  Note that both argument
     and result should be sign-extended under -fsigned-char,
     zero-extended under -fno-signed-char.  */
  HOST_WIDE_INT (*to_target_charset) (HOST_WIDE_INT);

  /* Pointers to machine-independent attribute tables, for front ends
     using attribs.c.  If one is NULL, it is ignored.  Respectively, a
     table of attributes specific to the language, a table of
     attributes common to two or more languages (to allow easy
     sharing), and a table of attributes for checking formats.  */
  const struct attribute_spec *attribute_table;
  const struct attribute_spec *common_attribute_table;
  const struct attribute_spec *format_attribute_table;

  /* Function-related language hooks.  */
  struct lang_hooks_for_functions function;

  struct lang_hooks_for_tree_inlining tree_inlining;

  struct lang_hooks_for_callgraph callgraph;

  struct lang_hooks_for_tree_dump tree_dump;

  struct lang_hooks_for_decls decls;

  struct lang_hooks_for_types types;

  /* Perform language-specific gimplification on the argument.  Returns an
     enum gimplify_status, though we can't see that type here.  */
  int (*gimplify_expr) (tree *, tree *, tree *);

  /* Fold an OBJ_TYPE_REF expression to the address of a function.
     KNOWN_TYPE carries the true type of the OBJ_TYPE_REF_OBJECT.  */
  tree (*fold_obj_type_ref) (tree, tree);

  /* Do language specific processing in the builtin function DECL  */
  tree (*builtin_function) (tree decl);

  /* Used to set up the tree_contains_structure array for a frontend. */
  void (*init_ts) (void);

  /* Called by recompute_tree_invariant_for_addr_expr to go from EXPR
     to a contained expression or DECL, possibly updating *TC, *TI or
     *SE if in the process TREE_CONSTANT, TREE_INVARIANT or
     TREE_SIDE_EFFECTS need updating.  */
  tree (*expr_to_decl) (tree expr, bool *tc, bool *ti, bool *se);

  /* Whenever you add entries here, make sure you adjust langhooks-def.h
     and langhooks.c accordingly.  */
};

/* Each front end provides its own.  */
extern const struct lang_hooks lang_hooks;
extern tree add_builtin_function (const char *name, tree type,
				  int function_code, enum built_in_class cl,
				  const char *library_name,
				  tree attrs);

#endif /* GCC_LANG_HOOKS_H */
