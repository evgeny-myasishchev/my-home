# Home light controller

The controller itself is Arduino nano.

## Usage

Make sure platformio is installed. Upload:

```
pio run -t upload -e nano
```

## Unit tests

### Running universal tests

Install native deps:
```
pio run -e universal
```

Make sure to have reflex installed

```
# Run once
make test

# Run all tests on every change
reflex $(cat .reflex) -- sh -c "make build-test && ./build/apps/program"

# Filter logger specific tests
reflex $(cat .reflex) -- sh -c "make build-test && ./build/apps/program --gtest_filter=logger*"

# Or with a helper script
tests-watch

tests-watch --gtest_filter=logger*
```

### Running platform tests

Make sure to attach arduino uno

```
pio run -e platform-test
```