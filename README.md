[<img src="https://assets.signaloid.io/add-to-signaloid-cloud-logo-dark-v6.png#gh-dark-mode-only" alt="[Add to signaloid.io]" height="30">](https://signaloid.io/repositories?connect=https://github.com/signaloid/Signaloid-Demo-Quantum-AVQE#gh-dark-mode-only)
[<img src="https://assets.signaloid.io/add-to-signaloid-cloud-logo-light-v6.png#gh-light-mode-only" alt="[Add to signaloid.io]" height="30">](https://signaloid.io/repositories?connect=https://github.com/signaloid/Signaloid-Demo-Quantum-AVQE#gh-light-mode-only)

# MICRO Benchmark: Accelerated Variational Quantum Eigensolver

Benchmark from Tsoutsouras et al. MICRO paper.[^0]

Implementation of the Quantum Phase Estimation (QPE) subroutine of the Accelerated Variational Quantum Eigensolver based on the work by Cruise et al.[^1]

This quantum algorithm calculates ground states of a quantum system Hamiltonian, $H$.
This benchmark tries to find the quantum state $ψ(k)$
that minimizes $\langle ψ(k)|H|ψ(k) \rangle$.
Typically, this uses rejection sampling to calculate $P(φ|E)$ from measurements of $P(E|φ)$ from a quantum phase estimation (QPE) circuit.

## Arguments

```
avqeOpt <mode>
	<mode>: 1 for explicit computation, 0 for implicit uncertainty tracking (0 is the default)
```


[^0]: Vasileios Tsoutsouras, Orestis Kaparounakis, Bilgesu Arif Bilgin, Chatura Samarakoon, James Timothy Meech, Jan Heck, Phillip Stanley-Marbell: The Laplace Microarchitecture for Tracking Data Uncertainty and Its Implementation in a RISC-V Processor. MICRO 2021: 1254-1269

[^1]: J. R. Cruise, N. I. Gillespie, and B. Reid: Practical Quantum Computing: The value of local computation. arXiv:2009.08513 [quant-ph], Sep. 2020, Accessed: Apr. 07, 2021. [Online]. Available: http://arxiv.org/abs/2009.08513
