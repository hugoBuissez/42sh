#!/bin/bash

source venv/bin/activate
pytest tests -s
deactivate
