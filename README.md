# $\rho$-index

A text index based on the _suffixient_ suffix array sample.
Basically a form of [r-index](https://github.com/nicolaprezza/r-index) [2] that can find MEMs [3], but:
+ smaller (hopefully), because the SA sample size probably significantly smaller
+ faster (hopefully), because it does not use backward stepping
+ less complex (subjectively)

## Installation
TODO

## Usage
TODO

## Examples
TODO

## Authors
Adrian Goga, Travis Gagie (original LCP), Nicola Prezza ([suffixient](https://github.com/nicolaprezza/suffixient)).

## License
[MIT](LICENSE)

## References
[1] Ahmed, O., Baláž, A., Brown, N. K., Depuydt, L., Goga, A., Petescia, A., Zakeri, M., Fostier, J., Gagie, T., Langmead, B., Navarro, G. & Prezza, N. (2023). r-indexing without backward searching. arXiv preprint arXiv:2312.01359.

[2] Gagie, Travis, Gonzalo Navarro, and Nicola Prezza. "Optimal-time text indexing in BWT-runs bounded space." Proceedings of the Twenty-Ninth Annual ACM-SIAM Symposium on Discrete Algorithms. Society for Industrial and Applied Mathematics, 2018.

[3] Rossi, Massimiliano, et al. "MONI: a pangenomic index for finding maximal exact matches." Journal of Computational Biology 29.2 (2022): 169-187.
