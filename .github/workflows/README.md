# CI with Github workflows

[//]: # (Comment)
[//]: # (This file when in the plugin-boilerplate repository is included in the Ethereum Plugin SDK Github page, keep that in mind when editing it.)

The following Github workflows are provided for your application:

## build_and_functional_tests.yml

Example of full integration of the build system and Ragger testing tool in the CI.
This workflow will run your functionnal tests and ensure they are passing on each pull-request.

While this workflow itself is not mandatory, some form of functional tests in CI is mandatory.

## cflite_cron.yml and cflite_pr.yml

This CIs will run the fuzzer tool on your plugin.

This CIs are mandatory.

## check_sdk.yml

This CI will ensure that you are using the latest version of the Ethereum Plugin SDK.

This CI is mandatory.

## guidelines_enforcer.yml

This CI will perform checks to ensure that you meet the Ledger application guidelines.

You can find more information [here](https://github.com/LedgerHQ/ledger-app-workflows/)

This CI is mandatory.

## lint-workflow.yml

This CI will ensure that your application is properly linted.

While this CI is mandatory, the content of the `.clang-format` file is not and you can tweak it to your liking.
