========================
Monte Carlo Integration
========================

Basic Information
=================

Monte Carlo Methods or Monte Carlo experiments are based on repeated random sampling.  They can be applied to complicated deterministic problems like optimization,  simulations or integration, which would otherwise be infeasible to compute in an acceptable time. Using random sampling simplifies these problems significantly.

Problem to Solve
==================

In a lot of scenarios using integration methods like Trapezoid or
Simpsons rule are more efficient than Monte Carlo integration. Furthermore
these methods produce deterministic results, which are a lot easier to work
with compared to random ones. On the downside these methods suffer from the curse
of dimensionality. Therefore Monte Carlo Integration (and Monte Carlo methods in general)
are usually used, when we decide that random approximations are better
than forgoing results altogether. The power of the Monte Carlo estimator
comes from its incredible simplicity.


Challenges:

- The biggest challenge is have a good random number generator, the goal is to use all little random points as possible to get a good estimate
- Another challenge in this project is to define a suitable testing strategy, since the estimator does not produce deterministic results

Prospective Users
=================

Prospecitive users are generally consists of people who want to calculate
integrals and can accept the uncertainty in the results of Monte Carlo
Integration. It is expected that users are experienced in coding, thus there will
be no GUI, but a only an API.

System Architecture
===================

System Architecture will be done during the project

API Description
===============

This API description is subject to change.

Python API:

- class: MonteCarloIntegration
- setIntegrand()
- setImportanceDistribution()
- setSamplingMethod()
- run()
- test()

C++ API:

- runMonteCarloIntegration(integrand, importanceDistribution, samplingMethod)
- testMonteCarloIntegration(integrand, importanceDistribution, samplingMethod)


Engineering Infrastructure
==========================

Automatic build system: make

For the build system "make" will be used. For easy understanding and maintenance,
the Makefile will be self created. In case a more complex Makefile is necessarry,
including CMalke to create the Makefile will be considered.

Version control: git

Git will be used as a version control system. Since this project only has one author,
there will be a master and a development branch will be set up. In case it is necessarry, and additional release/testing branch can be implemented, in order to only
merge runnig code into master from there.

Testing framework: pytest

The mains testing framework will be pytest. Since the project deals with a non-deterministic method, a suitable testing strategy still has to be developed.
For C++ Catch can be used as a testing Framework.

Documentation: README.md

It is expected that the documentation will be rather concise, thus a simple readme.md
file should be sufficient.


Schedule
========


* Planning phase (6 weeks from 9/19 to 10/31):
* Week 1 (10/31): 
- Build concept how math should be implemented
- Start developing a testing strategy

* Week 2 (11/7):
- Prototyping classes
- Get used to new tools e.g. Catch
- Set up version control, environments, build tools, testing framework, etc.
- Elaborate architecture and scope
- Elaborate and define testing strategy

* Week 3 (11/14):
- Start developing C++ code and Python wrapper

* Week 4 (11/21):
- Code C++ code and Python wrapper
- Start writing tests and start testing
- Debugging

* Week 5 (11/28):
- Finish with C++ Code and Python wrapper
- Keep testing
- Debugging
- Start documentation

* Week 6 (12/5):
- Finish Debugging
- Finish testing
- Elaborate documentation

* Week 7 (12/12):
- Buffer time
- Start working on the final presentation

* Week 8 (12/19): 
- Prepare final presentation

References
==========

List the external references for the information provided in the proposal.
