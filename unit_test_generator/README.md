# Unit Test Generation - Org Chart API

This tool implements a complete unit test generation workflow for the Org Chart API C++ project using **Google Gemini 2.0 Flash** (GitHub-hosted model).

## 🚀 Updated Architecture & Workflow (2024)

This tool now matches the provided architecture diagram **exactly**. The pipeline is fully automated and uses strict YAML prompts for each LLM step.

### Default Model
- **Gemini 2.0 Flash** (self-hosted or API)

### New YAML Prompt Files
The following YAML files in `unit_test_generator/instructions/` provide strict instructions for each LLM step:
- `refactor_prompt.yaml`: Refactor C++ code for testability (no logic changes)
- `deduplicate_prompt.yaml`: Remove duplicate/redundant tests
- `add_libraries_prompt.yaml`: Add missing #include/library dependencies
- `uncovered_prompt.yaml`: Generate new tests for uncovered code (coverage-driven)

### Updated Pipeline Steps
1. **Replica Creation**: Copies each C++ file as a test replica for processing
2. **Refactor for Testability**: LLM refactors code for better testability using `refactor_prompt.yaml`
3. **Initial Test Generation**: LLM generates initial tests using `initial_prompt.yaml`
4. **Deduplication (LLM)**: LLM removes duplicate/redundant tests using `deduplicate_prompt.yaml`
5. **Deduplication (Script)**: Script-side deduplication for extra safety
6. **Add Missing Libraries (LLM)**: LLM adds missing includes/libraries using `add_libraries_prompt.yaml` and build logs
7. **Build & Debug**: Automated build/test, LLM fixes errors if build fails
8. **Coverage Analysis**: Runs GNU gcov, saves coverage report
9. **Coverage-Driven New Test Generation**: LLM generates new tests for uncovered code using `uncovered_prompt.yaml`
10. **Final Output**: All test files, coverage, and test report are saved

### How YAML Prompts Are Used
- Each step in the pipeline uses a dedicated YAML file to instruct the LLM with strict, repeatable guidance.
- Prompts are designed to ensure no duplicate tests, all required libraries are included, and coverage is maximized.

### Model Version
- All references and defaults now use **Gemini 2.0 Flash**.

## Test Requirements Implementation

### ✅ Setup the Environment
- **Working directory**: `unit_test_generator/` folder
- **Self-hosted LLM**: Google Gemini 2.0 Flash API (GitHub-hosted model)
- **C++ compiler**: Available in WSL environment
- **Testing framework**: Drogon framework (not Google Test)

### ✅ Initial Test Generation
- **Input**: C++ project files (controllers/, models/, utils/)
- **Process**: Sends code to LLM with YAML strict instruction file
- **Output**: Initial unit tests saved in `generated_tests/` folder

### ✅ Refine the Tests
- **Process**: Sends generated tests back to LLM for refinement
- **Actions**: Removes duplicates, adds libraries, improves formatting
- **Output**: Refined unit test files for all input files

### ✅ Build and Debug
- **Process**: Builds project with generated tests
- **Error handling**: If build fails, sends files and logs to LLM with YAML prompt
- **Coverage**: If build passes, calculates test coverage using GNU tools
- **Improvement**: Sends coverage data back to LLM for further improvements

### ✅ Final Output
- **Test folder**: `unit_test_generator/generated_tests/` with properly formatted tests
- **Coverage report**: `unit_test_generator/coverage/coverage_report.txt`
- **Test report**: `unit_test_generator/coverage/test_report.txt`

## Requirements Met

- ✅ **YAML files**: Strict instructions for LLM guidance
- ✅ **GNU coverage tools**: Integration with gcov for coverage analysis
- ✅ **Library handling**: Automatic inclusion of necessary Drogon libraries
- ✅ **Duplicate rejection**: Removes redundant tests automatically
- ✅ **Simple process**: One-command execution with `generate_tests_github.py`
- ✅ **Repeatable**: Consistent results across multiple runs

## Features

- ✅ **Fast & Reliable** - No timeout issues like Ollama
- ✅ **Multi-Model Support** - Choose between Gemini (recommended) or OpenAI
- ✅ **Drogon Framework Support** - Specialized for Drogon C++ web framework
- ✅ **Complete Pipeline** - Initial generation → Refinement → Build → Coverage → Final refinement
- ✅ **Error Recovery** - Automatic build error fixing
- ✅ **Coverage Analysis** - Code coverage reporting and improvement

## Model Comparison

| Feature | Gemini | OpenAI |
|---------|--------|---------|
| Speed | ⚡ Very Fast (1-3s) | ⚡ Fast (2-5s) |
| Cost | 💰 Very Cheap ($0.001/1K tokens) | 💰 Cheap ($0.03/1K tokens) |
| Reliability | ✅ High | ✅ High |
| C++ Understanding | ✅ Excellent | ✅ Excellent |
| Setup | 🚀 Easy | 🚀 Easy |

**Recommendation**: Use **Gemini** - it's faster, cheaper, and equally capable for C++ test generation.

## Setup (WSL)

### 1. Get Gemini API Key
1. Visit [Google AI Studio](https://makersuite.google.com/app/apikey)
2. Create an account and get an API key
3. Free tier: 15 requests/minute, 1M characters/month

### 2. Configure API Key
Run the setup script:
```bash
python3 unit_test_generator/setup_github.py
```

Or create a .env file manually:
```bash
# Create .env file in unit_test_generator/ folder
echo "GEMINI_API_KEY=your-gemini-key-here" > unit_test_generator/.env
```

### 3. Install Dependencies
```bash
pip3 install requests
```

### 4. Verify WSL Environment
```bash
# Check Python version
python3 --version

# Check if Drogon is available
pkg-config --exists drogon && echo "Drogon found" || echo "Drogon not found"

# Check if gcov is available
which gcov && echo "GNU coverage tools available" || echo "GNU coverage tools not found"
```

## Running the Test Generation Pipeline (WSL)

### Quick Start
```bash
# Run the complete test generation pipeline
python3 unit_test_generator/generate_tests_github.py
```

This single command executes all test generation steps:
1. **Environment Setup** - Verify API key and dependencies
2. **Initial Test Generation** - Generate tests for all C++ files
3. **Refine Tests** - Remove duplicates, add libraries, improve quality
4. **Build and Debug** - Build project, fix errors automatically
5. **Coverage Analysis** - Run GNU coverage tools (gcov)
6. **Final Refinement** - Improve tests based on coverage feedback
7. **Generate Reports** - Create coverage and test reports

### Output Files
- **Generated tests**: `unit_test_generator/generated_tests/`
- **Coverage report**: `unit_test_generator/coverage/coverage_report.txt`
- **Test report**: `unit_test_generator/coverage/test_report.txt`

## File Structure

```
unit_test_generator/
├── generate_tests_github.py      # Main test generator (supports both models)
├── setup_github.py              # Setup script
├── README.md                    # This file
├── .env                         # Gemini API key (create this file)
├── instructions/
│   ├── initial_prompt.yaml      # Initial test generation
│   ├── refactor_prompt.yaml     # Testability refactor
│   ├── deduplicate_prompt.yaml  # Deduplication
│   ├── add_libraries_prompt.yaml# Library fixing
│   └── uncovered_prompt.yaml    # Coverage-driven new tests
├── generated_tests/             # Output: Generated test files
└── coverage/                    # Output: Coverage and test reports
```

## Supported C++ Files

The generator processes:
- `main.cc` - Main application file
- `controllers/*.cc` - HTTP controllers
- `models/*.cc` - Data models
- `utils/*.cc` - Utility functions

## Test Framework

Uses **Drogon test framework** (not Google Test):
- `#include <drogon/drogon.h>`
- `#include <drogon/drogon_test.h>`
- `TEST()` macro for test functions
- Proper HTTP request/response mocking

## Cost Estimation

### Gemini (Recommended)
- **Cost**: ~$0.001 per 1K tokens
- **Typical cost per file**: $0.01-0.05
- **Total project cost**: ~$0.50-1.00 for complete test generation

### OpenAI
- **Cost**: ~$0.03 per 1K tokens
- **Typical cost per file**: $0.10-0.30
- **Total project cost**: ~$2-5 for complete test generation

## Environment Variables

| Variable | Description | Default |
|----------|-------------|---------|
| `LLM_MODEL_TYPE` | Model to use: 'gemini' or 'openai' | 'gemini' |
| `GEMINI_API_KEY` | Your Gemini API key | Required if using Gemini |
| `OPENAI_API_KEY` | Your OpenAI API key | Required if using OpenAI |
| `GEMINI_MODEL` | Gemini model name | 'gemini-2.0-flash' |
| `OPENAI_MODEL` | OpenAI model name | 'gpt-4' |

## Troubleshooting (WSL)

### API Key Issues
- Ensure API key is valid and has credits
- Check internet connection
- Verify API key format (Gemini: Usually starts with `AIza...`)

### WSL Environment Issues
```bash
# Check if running in WSL
uname -a

# Verify Python installation
python3 --version

# Check Drogon installation
pkg-config --exists drogon && echo "Drogon found" || echo "Install Drogon first"

# Verify GNU coverage tools
which gcov && echo "gcov available" || echo "Install build-essential"
```

### Build Errors
- The system automatically attempts to fix build errors
- Check generated test files for syntax issues
- Ensure Drogon framework is properly installed in WSL
- Run `./build.sh` manually to test build process

### Coverage Issues
- Coverage reports are generated using `gcov`
- Ensure project is built with coverage flags
- Check that test files are properly linked
- Verify WSL has build tools: `sudo apt install build-essential`

## Comparison with Ollama

| Feature | Gemini/OpenAI | Ollama |
|---------|---------------|---------|
| Speed | ⚡ Fast (1-5s) | 🐌 Slow (30s+) |
| Reliability | ✅ High | ⚠️ Timeout issues |
| Cost | 💰 $0.50-5 | 🆓 Free |
| Privacy | 🌐 External | 🔒 Local |
| Setup | 🚀 Easy | 🔧 Complex |

## Test Generation Requirements Met

✅ **Setup Environment** - WSL with GitHub-hosted models  
✅ **Initial Test Generation** - YAML prompts + LLM generation  
✅ **Refine Tests** - Remove duplicates, improve quality  
✅ **Build and Debug** - Automatic build + error fixing  
✅ **Final Output** - Coverage reports + refined tests  

The system fully automates the unit test generation process as specified in the requirements. 