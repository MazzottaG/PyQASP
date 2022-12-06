cmake_minimum_required(VERSION 3.14)
#if (WIN32)
#    SET(CMAKE_C_COMPILER "C:/mingw-w64/i686-8.1.0-posix-dwarf-rt_v6-rev0/mingw32/bin/gcc.exe")
#    SET(CMAKE_CXX_COMPILER "C:/mingw-w64/i686-8.1.0-posix-dwarf-rt_v6-rev0/mingw32/bin/g++.exe")
#endif (WIN32)

if (UNIX)
    SET(CMAKE_C_COMPILER "gcc")
    SET(CMAKE_CXX_COMPILER "g++")

    set(FLAGS "-Wall -std=c++11 -g3")
    add_definitions(${FLAGS})
endif (UNIX)

project(DLV2)

set(CMAKE_CXX_STANDARD 14)
set(DEXT "")

set(LINK "")
list(APPEND LIBS zlibstatic)
list(APPEND DIRS)

set(HTD "0")
set(DLIB "0")
set(WITH-PYTHON_THREE "0")
set(WITH-PYTHON "0")
set(WITH-SPARQL "0")
set(WITH-SQL "0")

if(NOT DEFINED HTD)
    set(HTD "1")
endif(NOT DEFINED HTD)

if(NOT DEFINED DLIB)
    set(DLIB "1")
endif(NOT DEFINED DLIB)

if(NOT DEFINED WITH-PYTHON_THREE)
    set(WITH-PYTHON_THREE "1")
endif(NOT DEFINED WITH-PYTHON_THREE)

if(NOT DEFINED WITH-PYTHON)
    set(WITH-PYTHON "1")
endif(NOT DEFINED WITH-PYTHON)

if(NOT DEFINED WITH-SPARQL)
    set(WITH-SPARQL "1")
endif(NOT DEFINED WITH-SPARQL)

if(NOT DEFINED SQL)
    set(SQL "1")
endif(NOT DEFINED SQL)

set(CMAKE_EXE_LINKER_FLAGS "-static")


if (${DLIB})
    add_subdirectory(dlib dlib_build)
    list(APPEND LIBS dlib::dlib)
endif (${DLIB})

if (${HTD})
    add_subdirectory(htd htd_build)
    string(APPEND DEXT "-DHTD ")
    list(APPEND LIBS htd)
    list(APPEND DIRS htd/include)
    target_include_directories(htd PUBLIC htd/include)

endif (${HTD})

if (${WITH-PYTHON_THREE})
    set(PY_VERSION "3.9")
    find_package(Python3 ${PY_VERSION} EXACT REQUIRED COMPONENTS Development)
    find_package(PythonLibs ${PY_VERSION} REQUIRED)

    string(APPEND DEXT "-DPYTHON -DENABLE_PYTHON -DPYTHON_THREE ")
    #    add_subdirectory(python3 python3_build)
    #    list(APPEND LIBS libpython-static)
    #	 find_package (Python COMPONENTS Interpreter Development)
    #    find_package(Python3 COMPONENTS Development)
    #    list(APPEND LIBS Python3::Python Python3::Module)
    #    list(APPEND DIRS ${Python3_INCLUDE_DIRS})
    #    target_include_directories(DLV2 PRIVATE ${Python3_INCLUDE_DIRS})

    #string(APPEND LINK "Python3::Module Python3::Python ")


elseif (${WITH-PYTHON})
    string(APPEND DEXT "-DPYTHON -DENABLE_PYTHON ")
    find_package(Python COMPONENTS Development)
    find_package(PythonLibs REQUIRED)
    list(APPEND LIBS Python::Python)
    list(APPEND DIRS ${Python_INCLUDE_DIRS})
    #    target_include_directories(DLV2 PRIVATE ${Python_INCLUDE_DIRS})
endif (${WITH-PYTHON_THREE})

if (${WITH-SPARQL})
    string(APPEND DEXT "-DSPARQL ")
endif (${WITH-SPARQL})

if (${WITH-SQL})
    string(APPEND DEXT "-DSQL")
    find_package(ODBC)
    message(WARNING ${ODBC_LIBRARIES})
endif (${WITH-SQL})

#message(WARNING "DEXT: ${DEXT}")

add_definitions(${DEXT})



#message(WARNING "COMPILATORE: ${CMAKE_CXX_COMPILER} ${FLAGS}")


add_definitions(-DNDEBUG -O3)
add_subdirectory(zlib zlib_build)


include_directories(src/common)
include_directories(src/idlv)
include_directories(src/idlv/directives)
include_directories(src/idlv/grounder)
include_directories(src/idlv/grounder/atom)
include_directories(src/idlv/grounder/atom/AggregateUtility)
include_directories(src/idlv/grounder/atom/externalAtom)
include_directories(src/idlv/grounder/exception)
include_directories(src/idlv/grounder/ground)
include_directories(src/idlv/grounder/hash)
include_directories(src/idlv/grounder/output)
include_directories(src/idlv/grounder/statement)
include_directories(src/idlv/grounder/table)
include_directories(src/idlv/grounder/term)
include_directories(src/idlv/input)
include_directories(src/idlv/subsumption)
include_directories(src/idlv/util)
include_directories(src/wasp)
include_directories(src/wasp/heuristic)
include_directories(src/wasp/interpreters)
include_directories(src/wasp/preasp)
include_directories(src/wasp/preasp/output)
include_directories(src/wasp/propagators)
include_directories(src/wasp/stl)
include_directories(src/wasp/util)
include_directories(src/wasp/weakconstraints)

add_executable(dlv2
        src/common/Assert.h
        src/common/DLV2Facade.cpp
        src/common/DLV2Facade.h
        src/common/ErrorMessage.h
        src/common/Help.h
        src/common/Options.cpp
        src/common/Options.h
        src/common/Trace.h
        src/common/Utility.h
        src/idlv/directives/DBConnection.cpp
        src/idlv/directives/DBConnection.h
        src/idlv/directives/DBDirective.h
        src/idlv/directives/Directive.h
        src/idlv/directives/DirectiveFactory.h
        src/idlv/directives/DirectivesHolder.cpp
        src/idlv/directives/DirectivesHolder.h
        src/idlv/directives/ExportDirective.h
        src/idlv/directives/ExtPredConversionDirective.cpp
        src/idlv/directives/ExtPredConversionDirective.h
        src/idlv/directives/ImportDirective.h
        src/idlv/directives/Query.h
        src/idlv/directives/ShowDirective.cpp
        src/idlv/directives/ShowDirective.h
        src/idlv/directives/SparqlConnection.cpp
        src/idlv/directives/SparqlConnection.h
        src/idlv/directives/SparqlDirective.h
        src/idlv/grounder/atom/AggregateAtom.cpp
        src/idlv/grounder/atom/AggregateAtom.h
        src/idlv/grounder/atom/AggregateElement.cpp
        src/idlv/grounder/atom/AggregateElement.h
        src/idlv/grounder/atom/AggregateUtility/AllCombinationsChainGenerator.cpp
        src/idlv/grounder/atom/AggregateUtility/ChainElementCombsGenerator.cpp
        src/idlv/grounder/atom/AggregateUtility/ChainElementCombsGenerator.h
        src/idlv/grounder/atom/AggregateUtility/CombsGenerator.cpp
        src/idlv/grounder/atom/AggregateUtility/CombsGenerator.h
        src/idlv/grounder/atom/AggregateUtility/CountCombsGenerator.cpp
        src/idlv/grounder/atom/AggregateUtility/MinMaxCombsGenerator.cpp
        src/idlv/grounder/atom/AggregateUtility/SumCombsChainGenerator.cpp
        src/idlv/grounder/atom/AggregateUtility/SumCombsGenerator.cpp
        src/idlv/grounder/atom/Atom.cpp
        src/idlv/grounder/atom/Atom.h
        src/idlv/grounder/atom/BuiltInAtom.cpp
        src/idlv/grounder/atom/BuiltInAtom.h
        src/idlv/grounder/atom/Choice.cpp
        src/idlv/grounder/atom/Choice.h
        src/idlv/grounder/atom/ChoiceElement.cpp
        src/idlv/grounder/atom/ChoiceElement.h
        src/idlv/grounder/atom/ClassicalLiteral.cpp
        src/idlv/grounder/atom/ClassicalLiteral.h
        src/idlv/grounder/atom/ExtAtom.cpp
        src/idlv/grounder/atom/ExtAtom.h
        src/idlv/grounder/atom/externalAtom/AbsExtAtom.h
        src/idlv/grounder/atom/externalAtom/AppendExtAtom.cpp
        src/idlv/grounder/atom/externalAtom/AppendExtAtom.h
        src/idlv/grounder/atom/externalAtom/AppendListExtAtom.h
        src/idlv/grounder/atom/externalAtom/ArithmeticExtAtom.cpp
        src/idlv/grounder/atom/externalAtom/ArithmeticExtAtom.h
        src/idlv/grounder/atom/externalAtom/BaseSubStrExtAtom.cpp
        src/idlv/grounder/atom/externalAtom/BaseSubStrExtAtom.h
        src/idlv/grounder/atom/externalAtom/DelNthListExtAtom.h
        src/idlv/grounder/atom/externalAtom/FlattenListExtAtom.h
        src/idlv/grounder/atom/externalAtom/HeadListExtAtom.h
        src/idlv/grounder/atom/externalAtom/InsLastListExtAtom.h
        src/idlv/grounder/atom/externalAtom/InsNthListExtAtom.h
        src/idlv/grounder/atom/externalAtom/IntExtAtom.cpp
        src/idlv/grounder/atom/externalAtom/IntExtAtom.h
        src/idlv/grounder/atom/externalAtom/LastListExtAtom.h
        src/idlv/grounder/atom/externalAtom/Length32ExtAtom.cpp
        src/idlv/grounder/atom/externalAtom/Length32ExtAtom.h
        src/idlv/grounder/atom/externalAtom/LengthExtAtom.h
        src/idlv/grounder/atom/externalAtom/LengthExtAtomUTF16.h
        src/idlv/grounder/atom/externalAtom/LengthListExtAtom.h
        src/idlv/grounder/atom/externalAtom/LengthStringExtAtom.cpp
        src/idlv/grounder/atom/externalAtom/LengthStringExtAtom.h
        src/idlv/grounder/atom/externalAtom/ListExtAtom.cpp
        src/idlv/grounder/atom/externalAtom/ListExtAtom.h
        src/idlv/grounder/atom/externalAtom/MemberExtAtom.cpp
        src/idlv/grounder/atom/externalAtom/MemberExtAtom.h
        src/idlv/grounder/atom/externalAtom/MemberListExtAtom.cpp
        src/idlv/grounder/atom/externalAtom/MemberListExtAtom.h
        src/idlv/grounder/atom/externalAtom/MemberNthListExtAtom.h
        src/idlv/grounder/atom/externalAtom/ModExtAtom.h
        src/idlv/grounder/atom/externalAtom/RandExtAtom.cpp
        src/idlv/grounder/atom/externalAtom/RandExtAtom.h
        src/idlv/grounder/atom/externalAtom/ReverseExtAtom.h
        src/idlv/grounder/atom/externalAtom/ReverseListExtAtom.h
        src/idlv/grounder/atom/externalAtom/SelectListExtAtom.h
        src/idlv/grounder/atom/externalAtom/SimpleReverseListExtAtom.h
        src/idlv/grounder/atom/externalAtom/SimpleSelectListExtAtom.cpp
        src/idlv/grounder/atom/externalAtom/SimpleSelectListExtAtom.h
        src/idlv/grounder/atom/externalAtom/StringExtAtom.cpp
        src/idlv/grounder/atom/externalAtom/StringExtAtom.h
        src/idlv/grounder/atom/externalAtom/SubListExtAtom.cpp
        src/idlv/grounder/atom/externalAtom/SubListExtAtom.h
        src/idlv/grounder/atom/externalAtom/SubStrExtAtom.cpp
        src/idlv/grounder/atom/externalAtom/SubStrExtAtom.h
        src/idlv/grounder/atom/externalAtom/SubStrExtAtomUTF16.cpp
        src/idlv/grounder/atom/externalAtom/SubStrExtAtomUTF16.h
        src/idlv/grounder/atom/externalAtom/SubStrExtAtomUTF32.cpp
        src/idlv/grounder/atom/externalAtom/SubStrExtAtomUTF32.h
        src/idlv/grounder/atom/externalAtom/SumExtAtom.h
        src/idlv/grounder/atom/externalAtom/TailListExtAtom.h
        src/idlv/grounder/atom/externalAtom/ToQuotedStrExtAtom.h
        src/idlv/grounder/atom/Predicate.h
        src/idlv/grounder/atom/PythonExtAtom.cpp
        src/idlv/grounder/atom/PythonExtAtom.h
        src/idlv/grounder/atom/PythonManager2.cpp
        src/idlv/grounder/atom/PythonManager2.h
        src/idlv/grounder/exception/ConstrainException.h
        src/idlv/grounder/exception/ExtAtomException.h
        src/idlv/grounder/exception/LpoptException.h
        src/idlv/grounder/exception/SafetyExceptionGrounder.h
        src/idlv/grounder/ground/BackJumpingGrounder.cpp
        src/idlv/grounder/ground/BackJumpingGrounder.h
        src/idlv/grounder/ground/BackTrackingGrounder.cpp
        src/idlv/grounder/ground/BackTrackingGrounder.h
        src/idlv/grounder/ground/DecompositionMapper.cpp
        src/idlv/grounder/ground/DecompositionMapper.h
        src/idlv/grounder/ground/DecompositionOptimizer.cpp
        src/idlv/grounder/ground/DecompositionOptimizer.h
        src/idlv/grounder/ground/IntelligentDecomposer.cpp
        src/idlv/grounder/ground/IntelligentDecomposer.h
        src/idlv/grounder/ground/MagicSets.cpp
        src/idlv/grounder/ground/MagicSets.h
        src/idlv/grounder/ground/NonGroundSimplifier.cpp
        src/idlv/grounder/ground/NonGroundSimplifier.h
        src/idlv/grounder/ground/ProgramGrounder.cpp
        src/idlv/grounder/ground/ProgramGrounder.h
        src/idlv/grounder/ground/PropagatorsHeuristicsHolder.cpp
        src/idlv/grounder/ground/PropagatorsHeuristicsHolder.h
        src/idlv/grounder/ground/PropositionalGrounder.cpp
        src/idlv/grounder/ground/PropositionalGrounder.h
        src/idlv/grounder/ground/RewriteMagic.cpp
        src/idlv/grounder/ground/RewriteMagic.h
        src/idlv/grounder/ground/RewriteMagicExternalSources.cpp
        src/idlv/grounder/ground/RewriteMagicExternalSources.h
        src/idlv/grounder/ground/RuleComplexityEvaluator.cpp
        src/idlv/grounder/ground/RuleComplexityEvaluator.h
        src/idlv/grounder/ground/StatementAtomMapping.cpp
        src/idlv/grounder/ground/StatementAtomMapping.h
        src/idlv/grounder/ground/StatementDependency.cpp
        src/idlv/grounder/ground/StatementDependency.h
        src/idlv/grounder/hash/Hashable.h
        src/idlv/grounder/hash/HashString.cpp
        src/idlv/grounder/hash/HashString.h
        src/idlv/grounder/hash/HashVecInt.cpp
        src/idlv/grounder/hash/HashVecInt.h
        src/idlv/grounder/output/IntermediateFormatOutputBuilder.cpp
        src/idlv/grounder/output/IntermediateFormatOutputBuilder.h
        src/idlv/grounder/output/NumericOutputBuilder.cpp
        src/idlv/grounder/output/NumericOutputBuilder.h
        src/idlv/grounder/output/NumericOutputBuilderStream.cpp
        src/idlv/grounder/output/NumericOutputBuilderStream.h
        src/idlv/grounder/output/OutputBuilder.cpp
        src/idlv/grounder/output/OutputBuilder.h
        src/idlv/grounder/output/PreaspOutputBuilder.cpp
        src/idlv/grounder/output/PreaspOutputBuilder.h
        src/idlv/grounder/output/TextualOutputBuilder.h
        src/idlv/grounder/statement/GroundingPreferences.cpp
        src/idlv/grounder/statement/GroundingPreferences.h
        src/idlv/grounder/statement/InputRewriter.cpp
        src/idlv/grounder/statement/InputRewriter.h
        src/idlv/grounder/statement/OrderRule.cpp
        src/idlv/grounder/statement/OrderRule.h
        src/idlv/grounder/statement/OrderRuleGroundable.cpp
        src/idlv/grounder/statement/OrderRuleGroundable.h
        src/idlv/grounder/statement/Rule.cpp
        src/idlv/grounder/statement/Rule.h
        src/idlv/grounder/statement/RuleStatistics.cpp
        src/idlv/grounder/statement/RuleStatistics.h
        src/idlv/grounder/table/AdvancedArray.h
        src/idlv/grounder/table/HistoryStructure.h
        src/idlv/grounder/table/IdGenerator.cpp
        src/idlv/grounder/table/IdGenerator.h
        src/idlv/grounder/table/IndexingStructure.cpp
        src/idlv/grounder/table/IndexingStructure.h
        src/idlv/grounder/table/PredicateExtension.cpp
        src/idlv/grounder/table/PredicateExtension.h
        src/idlv/grounder/table/PredicateTable.cpp
        src/idlv/grounder/table/PredicateTable.h
        src/idlv/grounder/table/TermTable.cpp
        src/idlv/grounder/table/TermTable.h
        src/idlv/grounder/term/ArithTerm.cpp
        src/idlv/grounder/term/ArithTerm.h
        src/idlv/grounder/term/ConstantTerm.cpp
        src/idlv/grounder/term/ConstantTerm.h
        src/idlv/grounder/term/FunctionTerm.cpp
        src/idlv/grounder/term/FunctionTerm.h
        src/idlv/grounder/term/ListTerm.cpp
        src/idlv/grounder/term/ListTerm.h
        src/idlv/grounder/term/RangeTerm.h
        src/idlv/grounder/term/Term.cpp
        src/idlv/grounder/term/Term.h
        src/idlv/grounder/term/VariableTerm.h
        src/idlv/input/aspcore2.l
        src/idlv/input/aspcore2.y
        src/idlv/input/aspcore2_lexer.hpp
        src/idlv/input/aspcore2_parser.h
        src/idlv/input/aspcore2_parser.hpp
        src/idlv/input/Buffer.cpp
        src/idlv/input/Buffer.h
        src/idlv/input/EmptyInputBuilder.h
        src/idlv/input/ExtAtomFactory.cpp
        src/idlv/input/ExtAtomFactory.h
        src/idlv/input/InMemoryInputBuilder.cpp
        src/idlv/input/InMemoryInputBuilder.h
        src/idlv/input/InputBuilder.h
        src/idlv/input/InputDirector.cpp
        src/idlv/input/InputDirector.h
        src/idlv/input/ParserConstraint.h
        src/idlv/input/ParserConstraintAspCore2Strict.h
        src/idlv/input/PrinterInputBuilder.cpp
        src/idlv/input/PrinterInputBuilder.h
        src/idlv/input/SelectorBuilder.cpp
        src/idlv/input/SelectorBuilder.h
        src/idlv/main.cpp
        src/idlv/subsumption/RuleHashBuilder.h
        src/idlv/subsumption/SubsumptionChecker.cpp
        src/idlv/subsumption/SubsumptionChecker.h
        src/idlv/util/Constants.h
        src/idlv/util/Equation.h
        src/idlv/util/Graph.cpp
        src/idlv/util/Graph.h
        src/idlv/util/InteractiveStats.cpp
        src/idlv/util/InteractiveStats.h
        src/idlv/util/Timer.cpp
        src/idlv/util/Timer.h
        src/idlv/util/Utils.cpp
        src/idlv/util/Utils.h
        src/idlv/util/VecStack.h
        src/idlv/util/WarningMessage.h
        src/wasp/AnswerSetListener.h
        src/wasp/CautiousReasoning.cpp
        src/wasp/CautiousReasoning.h
        src/wasp/Clause.cpp
        src/wasp/Clause.h
        src/wasp/Enumeration.cpp
        src/wasp/Enumeration.h
        src/wasp/GUSData.cpp
        src/wasp/GUSData.h
        src/wasp/heuristic/ExternalHeuristic.cpp
        src/wasp/heuristic/ExternalHeuristic.h
        src/wasp/heuristic/MinisatHeuristic.cpp
        src/wasp/heuristic/MinisatHeuristic.h
        src/wasp/interpreters/Interpreter.cpp
        src/wasp/interpreters/Interpreter.h
        src/wasp/interpreters/MyPerlInterpreter.cpp
        src/wasp/interpreters/MyPerlInterpreter.h
        src/wasp/interpreters/MyPythonInterpreter.cpp
        src/wasp/interpreters/MyPythonInterpreter.h
        src/wasp/Learning.cpp
        src/wasp/Learning.h
        src/wasp/Literal.cpp
        src/wasp/Literal.h
        src/wasp/main.cc
        src/wasp/ParacoherentSemantics.cpp
        src/wasp/ParacoherentSemantics.h
        src/wasp/preasp/Aggregate.cpp
        src/wasp/preasp/Aggregate.h
        src/wasp/preasp/AtomData.h
        src/wasp/preasp/BinaryConstraint.cpp
        src/wasp/preasp/BinaryConstraint.h
        src/wasp/preasp/Builder.cpp
        src/wasp/preasp/Builder.h
        src/wasp/preasp/Completion.cpp
        src/wasp/preasp/Completion.h
        src/wasp/preasp/DependencyGraph.cpp
        src/wasp/preasp/DependencyGraph.h
        src/wasp/preasp/Interpretation.h
        src/wasp/preasp/Literal.h
        src/wasp/preasp/MinimizeConstraint.cpp
        src/wasp/preasp/MinimizeConstraint.h
        src/wasp/preasp/Options.h
        src/wasp/preasp/output/GringoOutput.h
        src/wasp/preasp/output/StreamCreator.h
        src/wasp/preasp/output/VirtualClauseCreator.h
        src/wasp/preasp/output/WBOOutput.h
        src/wasp/preasp/Parser.cpp
        src/wasp/preasp/Parser.h
        src/wasp/preasp/ParseUtils.h
        src/wasp/preasp/Program.h
        src/wasp/preasp/Rule.cpp
        src/wasp/preasp/Rule.h
        src/wasp/preasp/RuleData.h
        src/wasp/preasp/Simplifications.cpp
        src/wasp/preasp/Simplifications.h
        src/wasp/preasp/Trie.h
        src/wasp/preasp/Type.h
        src/wasp/PredicateMinimization.cpp
        src/wasp/PredicateMinimization.h
        src/wasp/propagators/Aggregate.cpp
        src/wasp/propagators/Aggregate.h
        src/wasp/propagators/Component.cpp
        src/wasp/propagators/Component.h
        src/wasp/propagators/DisjunctionPropagator.cpp
        src/wasp/propagators/DisjunctionPropagator.h
        src/wasp/propagators/ExternalPropagator.cpp
        src/wasp/propagators/ExternalPropagator.h
        src/wasp/propagators/HCComponent.cpp
        src/wasp/propagators/HCComponent.h
        src/wasp/propagators/MultiAggregate.cpp
        src/wasp/propagators/MultiAggregate.h
        src/wasp/propagators/PostPropagator.h
        src/wasp/propagators/Propagator.h
        src/wasp/propagators/ReductBasedCheck.cpp
        src/wasp/propagators/ReductBasedCheck.h
        src/wasp/propagators/UnfoundedBasedCheck.cpp
        src/wasp/propagators/UnfoundedBasedCheck.h
        src/wasp/Reason.h
        src/wasp/ReasonForBinaryClauses.cpp
        src/wasp/ReasonForBinaryClauses.h
        src/wasp/Restart.h
        src/wasp/Satelite.cpp
        src/wasp/Satelite.h
        src/wasp/Solver.cpp
        src/wasp/Solver.h
        src/wasp/stl/BoundedQueue.h
        src/wasp/stl/Heap.h
        src/wasp/stl/HeapLiterals.h
        src/wasp/stl/Trie.h
        src/wasp/stl/UnorderedSet.h
        src/wasp/stl/Vector.h
        src/wasp/stl/WatchedList.h
        src/wasp/util/ExtendedStatistics.cpp
        src/wasp/util/ExtendedStatistics.h
        src/wasp/util/Istream.h
        src/wasp/util/Statistics.cpp
        src/wasp/util/Statistics.h
        src/wasp/util/VariableNames.h
        src/wasp/util/WaspConstants.h
        src/wasp/Variables.h
        src/wasp/WaspConfiguration.cpp
        src/wasp/WaspConfiguration.h
        src/wasp/WaspCreator.h
        src/wasp/WaspFacade.cpp
        src/wasp/WaspFacade.h
        src/wasp/WaspOutput.h
        src/wasp/weakconstraints/AbstractWeakStrategy.cpp
        src/wasp/weakconstraints/AbstractWeakStrategy.h
        src/wasp/weakconstraints/CoreBased.cpp
        src/wasp/weakconstraints/CoreBased.h
        src/wasp/weakconstraints/K.cpp
        src/wasp/weakconstraints/K.h
        src/wasp/weakconstraints/One.cpp
        src/wasp/weakconstraints/One.h
        src/wasp/weakconstraints/Opt.cpp
        src/wasp/weakconstraints/Opt.h
        src/wasp/weakconstraints/PMRes.cpp
        src/wasp/weakconstraints/PMRes.h
        src/wasp/weakconstraints/WeakFacade.h
        src/main.cpp)

set(PROJ_LINK_FLAGS "Win64")
if (${WITH-PYTHON_THREE})
    target_link_libraries(dlv2 PRIVATE -static Python3::Module Python3::Python -DCMAKE_GENERATOR_PLATFORM=${PROJ_LINK_FLAGS} ${LIBS}  )
else()
    target_link_libraries(dlv2 PRIVATE -static -DCMAKE_GENERATOR_PLATFORM=${PROJ_LINK_FLAGS} ${LIBS}  )
endif (${WITH-PYTHON_THREE})
#target_compile_definitions(DLV2 PUBLIC "BOOST_PYTHON_STATIC_LIB")