# bloom-filter
This project is to explore and demonstrate the properties of the bloom filter.

# Sections
- Basic implementation and datagen
- Initial validation of expected behavior
- Validating Analytical
- Deriving Optimal Parameters Empirically
- Measuring performance benefits of application

## Basic Implementation
We have some helper objects.
We have this hashing function.
We can correctly store things and get some measure true-negatives.
Along the way, we have some utilities (datagen).
Assumptions: we're trusting murmur3, and the random generator in C++.
Ends with: a tour of the code and presenting the interface/parameters.

## Validating Basical Implementation
This is creating and running our core experiments.
Ends with: a way to populate our bloom filter, and compute its false-negative rate.

## Deriving Optimal Parameters Empirically
Create a loop that does some kind of gradient-descent over the parameters, or does a scan over parameters, and fill in the false-positive rate.
Ends with: a matrix of numbers, the rate.

## Measuring Performance Benefit of Application
Put the filter in front of an "expensive" set, and measure the cost of a negative. First with optimal parameters, and then with non-optimal parameters.

## Advanced Features
Computing the bloom filter in parallel, and other fun extensions.