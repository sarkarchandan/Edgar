# Edgar

#### This project attempts to create a small prototypical implementation for a columnar data store using the C++ Standard Template Library (STL). It lets users use T-SQL like query strings to store and retrieve data from the underlying in-memory data structure.

#### This project is actively in development. Underneath,  this is a shared library which uses CMake build system to build and integrate the [googletest](https://github.com/google/googletest "Googletest - Google Testing and Mocking Framework") in order to enable testing. The testing framework is closely integrated with the library during the CMake configuration time because the associated test suite is used both to validate the library features and at the same time it acts as a viable reference of how to use them. All exception cases are handled with std::exception and its child types. Following are the fundamental building blocks for the library. These may change during development. It uses the _database_ namespace to define all of its following underlying types.

## database::Container

##### The Container is the lowermost building block of the implementation layer which encapsulates the data as well as corresponding schema definition. For now, it stores all data in the form of a custom string type which uses the lexicographical\_compare utility of C++ language for comparison purpose, wherever applicable. Underlying schema definition of the container is a dictionary which defines the mapping between the column names and the constraint whether the column can be expressed in numbers or not. The assumption behind this construct is, a column which can be expressed in numbers, can be used for aggregate operations whereas a column having non-numeric value cannot be used for such operations. 

## database::TransactionFactory

##### The TransactionFactory is a friend class to the Container. It provides factory methods for creating as well as interacting with the Container. It defines the implementation layer methods for the CRUD operations, which involve the Container types. A particular query from the API layer is supposed to use one of several factory methods provided by the TransactionFactory to interact with a given container.

## database::Database

##### A Database maintains a collection of underlying containers. Therefore, an accurate form of API layer query must have the database name and container name in the predefined format for the query to be transferred to the accurate container. It uses a map data structure to maintain a mapping between the container name as the key and the container object as the corresponding value.

## database::DatabaseEngine

##### DatabaseEngine could be considered as the API layer for this library. DatabaseEngine should maintain a collection of Database objects, where each Database object, in turn, is supposed to maintain a collection of Container objects as explained above. it also uses the map data structure to maintain a mapping between a database name and the database object. The DatabaseEngine object, which implements various methods to interact with the underlying databases, provides two interfaces for queries. Queries are classified into data definition and data manipulation queries. The DatabaseEngine object provides two distinct methods to reason with the two distinct classes of queries.

## database::Query

##### The Query type uses the std::regex library and regular expression grammar of the C++ language. A query object can be created with a plain c-style string. When appropriate syntax is used, the query object would deconstruct the string and derive the query specification. As explained before the DatabaseEngine type provided two interfaces to accept given data definition or data manipulation query and interact with the underlying implementation layer.



#### This project is actively under development at the moment. Therefore, the definition and responsibilities for the aforementioned building blocks will be updated as progress are being made.

## Installation
##### Since this project is being built as a shared library, one can integrate the same with C++ project in one of the following ways using CMake.

```cmake
//A simple CMake project setup
src/main.cpp
build
CMakeLists.txt

//#1 Integration using git submodule //

//Creating the root level git repository and adding the Edgar as submodule
$ git init
$ git submodule add https://github.com/sarkarchandan/Edgar.git

//A simple CMake configuration going inside CMakeLists.txt
cmake_minimum_required(VERSION 3.1...3.13)
if(${CMAKE_VERSION} VERSION_LESS 3.13)
  cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()
project(Installation VERSION 1.0 DESCRIPTION "Installation of Edgar" LANGUAGES CXX)

#Adding extern/Edgar as subdirectory
add_subdirectory(extern/Edgar)

set(SOURCES src/main.cpp)
add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} Edgar)


//#2 Integration using  CMake FetchContent module //

//A simple CMake configuration going inside CMakeLists.txt
cmake_minimum_required(VERSION 3.1...3.13)
if(${CMAKE_VERSION} VERSION_LESS 3.13)
  cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()
project(Installation VERSION 1.0 DESCRIPTION "Installation of Edgar" LANGUAGES CXX)

include(FetchContent)

FetchContent_Declare(
  Edgar
  GIT_REPOSITORY https://github.com/sarkarchandan/Edgar.git
)

FetchContent_GetProperties(Edgar)
if(NOT Edgar_POPULATED)
  FetchContent_Populate(Edgar)
  add_subdirectory(${edgar_SOURCE_DIR} ${edgar_BINARY_DIR})
endif()

set(SOURCES src/main.cpp)
add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} Edgar)

//Assuming the out of source build
$ cd build
$ cmake -G "Unix Makefiles" ..
$ make

```

## Tests
##### Edgar provides a grouped and illustrative test suite. The test suite, on the other hand, works as secondary documentation of the library features. It uses the _googletest_ framework and CTest driver program to execute the tests. In this section, we describe the execution of the test suite assuming the steps described in the Installation section are performed. Based on which CMake integration option is used the location for the build directory of Edgar may differ. Samples of both cases are given.

```bash

//#1 Assuming that git submodule option is taken to link the library //
$ cd build/extern/Edgar/
$ make
$ ctest

//#2 Assuming that the CMake FetchContent module is used to link the library //

$ cd build/_deps/edgar-build/
$ make
$ ctest

```

## Examples
##### Following are some examples of constructing simple queries and using them for simple database operations to store and query data.  Examples will be updated with the ongoing work of implementing the features of the library.

```cpp
//Some example of simple database operations

//DatabaseEngine accepts data definition and data manipulation queries
database::DatabaseEngine database_engine;

//Create a Database
database::Query query_create_database = "create database company";
database_engine.ExecuteForDataDefinition(query_create_database,[&](bool database_created){
    ASSERT_TRUE(database_created);
});

//Create a Container
database::Query query_create_container = "create container company.employee(employee_id integer,employee_name string,employee_status string)";
database_engine.ExecuteForDataDefinition(query_create_container,[&](auto container_created) {
    ASSERT_TRUE(container_created);
});

//Insert data into Container
database::Query query_insert_into = "insert into company.employee values(employee_id:1,employee_name:chandan,employee_status:fulltime)";
database_engine.ExecuteForDataManipulation(query_insert_into,[&](auto query_result) {

//Snapshot of the inserted record will be available as the result of the query which has the type std::map<std::string,std::vector<std::string>>

/*
employee_id	{1}
employee_name	{chandan}
employee_status	{fulltime}
*/
});

//Inserting a set of records to the Container and fetch all
std::vector<database::Query> insert_queries = {
    "insert into company.employee values(employee_id:1,employee_name:Dominik_Schmidt,employee_status:fulltime)",
    "insert into company.employee values(employee_id:2,employee_name:Mathias_Heinrich,employee_status:parttime)",
    "insert into company.employee values(employee_id:3,employee_name:Andreas_Renner,employee_status:parttime)",
    "insert into company.employee values(employee_id:4,employee_name:Marcus_Kaiser,employee_status:fulltime)"
};

std::for_each(insert_queries.begin(),insert_queries.end(),[&](auto query) {
    database_engine.ExecuteForDataManipulation(query,[&](auto query_result) { /*pass*/ });
});

database::Query query_select_all = "select * from company.employee";
database_engine.ExecuteForDataManipulation(query_select_all,[&](auto query_result) {

//Result set for the select query, which has the type std::map<std::string,std::vector<std::string>>

/*
employee_id	{1 2 3 4}
employee_name	{Dominik_Schmidt Mathias_Heinrich Andreas_Renner Marcus_Kaiser}
employee_status	{fulltime parttime parttime fulltime}
*/
});

//Select a dataset by column names
database::Query query_create_database = "create database company";
database::Query query_create_container = "create container company.employee(employee_id integer,employee_name string,employee_status string)";
std::vector<database::Query> insert_queries = {
    "insert into company.employee values(employee_id:1,employee_name:Dominik_Schimdt,employee_status:fulltime)",
    "insert into company.employee values(employee_id:2,employee_name:Heinrich_Keil,employee_status:parttime)",
    "insert into company.employee values(employee_id:3,employee_name:Mathias_Rheinlein,employee_status:parttime)",
    "insert into company.employee values(employee_id:4,employee_name:Marcus_Eisermann,employee_status:fulltime)"
};
database_engine.ExecuteForDataDefinition(query_create_database,[&](auto database_created){ /*pass*/ });
database_engine.ExecuteForDataDefinition(query_create_container,[&](auto container_created){ /*pass*/ });
std::for_each(insert_queries.begin(),insert_queries.end(),[&](auto query) {
   database_engine.ExecuteForDataManipulation(query,[&](auto query_result) {
      /*pass*/
   });
});

database::Query query_select_dataset1 = "select employee_id,employee_name from company.employee";
database_engine.ExecuteForDataManipulation(query_select_dataset1,[&](auto query_result) {

//Result set for the select query, which has the type std::map<std::string,std::vector<std::string>>

employee_id	{1 2 3 4}
employee_name	{Dominik_Schimdt Heinrich_Keil Mathias_Rheinlein Marcus_Eisermann}

});

//Select records by a single criterion
database::Query query_create_database = "create database university";
database::Query query_create_container = "create container university.student (student_id integer,student_name string,student_dob string,student_gender string";
std::vector<database::Query> insert_queries = {
    "insert into university.student values(student_id:1,student_name:Markus_Eisermann,student_dob:19920312,student_gender:male)",
    "insert into university.student values(student_id:2,student_name:Ulrike_Von_Stryk,student_dob:19871112,student_gender:female)",
    "insert into university.student values(student_id:3,student_name:Dominik_Vlad,student_dob:19900821,student_gender:male)",
    "insert into university.student values(student_id:4,student_name:Mathias_Zeug,student_dob:19840918,student_gender:male)",
    "insert into university.student values(student_id:5,student_name:Kerstin_Peh,student_dob:19930611,student_gender:female)",
    "insert into university.student values(student_id:6,student_name:Tanja_Schimdt,student_dob:19951201,student_gender:female)",
    "insert into university.student values(student_id:7,student_name:Anna_Graser,student_dob:19890401,student_gender:female)",
    "insert into university.student values(student_id:8,student_name:Martin_Kaiser,student_dob:19840310,student_gender:male)",
};

database_engine.ExecuteForDataDefinition(query_create_database,[&](auto database_created) { /*pass*/ });
database_engine.ExecuteForDataDefinition(query_create_container,[&](auto container_created) { /*pass*/ });
std::for_each(insert_queries.begin(),insert_queries.end(),[&](auto query) {
   database_engine.ExecuteForDataManipulation(query,[&](auto query_result) {
      /*pass*/
   });
});

database::Query select_query1 = "select * from university.student where student_gender = female";
database_engine.ExecuteForDataManipulation(select_query1,[&](auto query_result) {

//Result set for the select query, which has the type std::map<std::string,std::vector<std::string>>

student_dob	{19871112 19930611 19951201 19890401}
student_gender	{female female female female}
student_id	{2 5 6 7}
student_name	{Ulrike_Von_Stryk Kerstin_Peh Tanja_Schimdt Anna_Graser}

});

//Examples will be refined and updated as the features of the library is implemented.
```


## Dependencies
* CMake version 3.11
* C++14

##### The library uses CMake [FetchContent](https://cmake.org/cmake/help/v3.11/module/FetchContent.html#id1 "CMake - FetchContent Module") module in order to integrate _googletest_ framework at configuration time. FetchContent is introduced with CMake version 3.11. The library uses features from C++ standard 14 in several places.

## Authors
* Chandan Sarkar

##### I am a grad student in computer science and learning C++ programming. This library is being created as part of my educational endeavors. I welcome all feedback and suggestions to improve my work.

## License
##### This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details.