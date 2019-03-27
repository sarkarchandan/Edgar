# Edgar

#### This project attempts to create a small prototypical implementation for a columnar data store using the C++ Standard Template Library (STL) . This project is actively in development. Underneath,  this is a shared library which uses CMake build system to build and integrate the [googletest](https://github.com/google/googletest "Googletest - Google Testing and Mocking Framework") in order to enable testing. The testing framework is closely integrated with the library during the CMake configuration time because the associated test suite is used both to validate the library features and at the same time it acts as a viable reference of how to use them. All exception cases are handled with std::exception and its child types. Following are the fundamental building blocks for the library. These may change during development. It uses the _database_ namespace to define all of its underlying types.

## database::Container

##### The Container is the lowermost building block of the implementation layer which encapsulates the data as well as corresponding schema definition. For now, it stores all data in the form of a custom string type which uses the lexicographical\_compare utility of C++ language for comparison purpose, wherever applicable. Underlying schema definition of the container is a dictionary which defines the mapping between the column names and the constraint whether the column is quantifiable or not. The assumption behind this construct is, a column which has quantifiable value can be used for aggregate operations whereas a column having non-quantifiable value cannot be used for such operations. 

## database::TransactionFactory

##### The TransactionFactory is a friend class to the Container. It provides factory methods for creating as well as interacting with the Container. It defines the implementation layer methods for the CRUD operations, which involve the Container types. A particular query from the API layer is supposed to use one of several factory methods provided by the TransactionFactory to interact with a given container.

## database::Database

##### A Database maintains a collection of underlying containers. Therefore, an accurate form of API layer query must have the database name and container name in the predefined format for the query to be transferred to the accurate container.

## database::DatabaseEngine

##### DatabaseEngine could be considered as the API layer for this library. DatabaseEngine should maintain a collection of Database types, where each Database type, in turn, is supposed to maintain a collection of Container types as mentioned before. It should provide all methods and abstractions necessary for the users to interact with the databases using queries with simple SQL like syntax.

## database::Query

##### The Query is the type which is supposed to exhaustively use the std::regex library and regular expression grammar of the C++ language in order to create seamless and easy to use interface for the API user to construct a query, which would then be consumed by the DatabaseEngine. DatabaseEngine would deconstruct the query and determine what operation should be performed on which database container.


### This project is actively under development at the moment. Therefore, the definition and responsibilities for the aforementioned building blocks will be updated as progress are being made.

## Installation
##### Will be updated soon.

## Tests
##### Will be updated soon.

## Examples
##### Will be updated soon.


## Dependencies
* CMake version 3.11
* C++14

##### The library uses CMake [FetchContent](https://cmake.org/cmake/help/v3.11/module/FetchContent.html#id1 "CMake - FetchContent Module") module in order to integrate _googletest_ framework at configuration time. FetchContent is introduced with CMake version 3.11. The library uses features from C++ standard 14 in several places.

## Authors
* Chandan Sarkar

##### I am a grad student in computer science and learning C++ programming. This library is being created as part of my educational endeavors. I welcome all feedback and suggestions to improve my work.

## License
##### This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details.