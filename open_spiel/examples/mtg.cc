// Copyright 2021 DeepMind Technologies Limited
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

// Execute as ./mtg --max_simulations=20 --num_games=10 --verbose=false --quiet=true
// Remember to compile with make mtg

#include <array>
#include <cstdio>
#include <map>
#include <memory>
#include <random>
#include <string>
#include <vector>
#include <algorithm>

#include "open_spiel/abseil-cpp/absl/container/btree_map.h"
#include "open_spiel/abseil-cpp/absl/flags/flag.h"
#include "open_spiel/abseil-cpp/absl/flags/parse.h"
#include "open_spiel/abseil-cpp/absl/strings/str_join.h"
#include "open_spiel/abseil-cpp/absl/time/clock.h"
#include "open_spiel/abseil-cpp/absl/time/time.h"
#include "open_spiel/algorithms/mcts.h"
#include "open_spiel/spiel.h"
#include "open_spiel/spiel_utils.h"

ABSL_FLAG(std::string, game, "magic_the_gathering", "The name of the game to play.");
ABSL_FLAG(std::string, player1, "mcts", "Who controls player1.");
ABSL_FLAG(std::string, player2, "mcts", "Who controls player2.");
ABSL_FLAG(double, uct_c, 2, "UCT exploration constant.");
ABSL_FLAG(int, rollout_count, 10, "How many rollouts per evaluation.");
ABSL_FLAG(int, max_simulations, 10000, "How many simulations to run.");
ABSL_FLAG(int, num_games, 100, "How many games to play.");
ABSL_FLAG(int, max_memory_mb, 1000,
          "The maximum memory used before cutting the search short.");
ABSL_FLAG(bool, solve, true, "Whether to use MCTS-Solver.");
ABSL_FLAG(uint_fast32_t, seed, 0, "Seed for MCTS.");
ABSL_FLAG(bool, verbose, true, "Show the MCTS stats of possible moves.");
ABSL_FLAG(bool, quiet, false, "Show the MCTS stats of possible moves.");

uint_fast32_t Seed()
{
  uint_fast32_t seed = absl::GetFlag(FLAGS_seed);
  return seed != 0 ? seed : absl::ToUnixMicros(absl::Now());
}

std::unique_ptr<open_spiel::Bot> InitBot(
    std::string type, const open_spiel::Game &game, open_spiel::Player player,
    std::shared_ptr<open_spiel::algorithms::Evaluator> evaluator)
{
  if (type == "random")
  {
    return open_spiel::MakeUniformRandomBot(player, Seed());
  }

  if (type == "mcts")
  {
    return std::make_unique<open_spiel::algorithms::MCTSBot>(
        game, std::move(evaluator), absl::GetFlag(FLAGS_uct_c),
        absl::GetFlag(FLAGS_max_simulations),
        absl::GetFlag(FLAGS_max_memory_mb), absl::GetFlag(FLAGS_solve), Seed(),
        absl::GetFlag(FLAGS_verbose));
  }
  open_spiel::SpielFatalError("Bad player type. Known types: mcts, random");
}

open_spiel::Action GetAction(const open_spiel::State &state,
                             std::string action_str)
{
  for (open_spiel::Action action : state.LegalActions())
  {
    if (action_str == state.ActionToString(state.CurrentPlayer(), action))
      return action;
  }
  return open_spiel::kInvalidAction;
}

std::pair<std::vector<double>, std::vector<std::string>> PlayGame(
    const open_spiel::Game &game,
    std::vector<std::unique_ptr<open_spiel::Bot>> &bots, std::mt19937 &rng,
    const std::vector<std::string> &initial_actions)
{
  bool quiet = absl::GetFlag(FLAGS_quiet);
  std::unique_ptr<open_spiel::State> state = game.NewInitialState();
  std::vector<std::string> history;

  for (const auto &action_str : initial_actions)
  {
    open_spiel::Action action = GetAction(*state, action_str);
    if (action == open_spiel::kInvalidAction)
      open_spiel::SpielFatalError(absl::StrCat("Invalid action: ", action_str));

    history.push_back(action_str);
    state->ApplyAction(action);
    if (!quiet)
    {
      std::cerr << "Forced action" << action_str << std::endl;
      std::cerr << "Next state:\n"
                << state->ToString() << std::endl;
    }
  }

  while (!state->IsTerminal())
  {
    open_spiel::Player player = state->CurrentPlayer();
    open_spiel::Action action;
    if (state->IsChanceNode())
    {
      // Chance node; sample one according to underlying distribution.
      open_spiel::ActionsAndProbs outcomes = state->ChanceOutcomes();
      action = open_spiel::SampleAction(outcomes, rng).first;
      if (!quiet)
        std::cerr << "Sampled action: " << state->ActionToString(player, action)
                  << std::endl;
    }
    else if (state->IsSimultaneousNode())
    {
      open_spiel::SpielFatalError(
          "MCTS not supported for games with simultaneous actions.");
    }
    else
    {
      // Decision node, ask the right bot to make its action
      action = bots[player]->Step(*state);
      if (!quiet)
      {
        std::string player_str = (player == 0) ? "\033[1;34mBot\033[0m" : "\033[1;31mEnemy\033[0m";
        int64_t min = 0;
        action = std::min(min, action - 1);
        std::string action_str = state->ActionToString(player, action);
        std::string action_color = "\033[1;32m"; // Default color for actions

        if (action < 7)
        {
          action_color = "\033[1;32m"; // Green for very low percentage actions
        }
        else if (action < 14)
        {
          action_color = "\033[1;33m"; // Yellow for low percentage actions
        }
        else if (action < 21)
        {
          action_color = "\033[1;36m"; // Cyan for medium-low percentage actions
        }
        else if (action < 28)
        {
          action_color = "\033[1;34m"; // Blue for medium percentage actions
        }
        else if (action < 35)
        {
          action_color = "\033[1;35m"; // Magenta for high percentage actions
        }
        else if (action < 300)
        {
          action_color = "\033[1;91m"; // Bright Red for 2XX actions
        }
        else if (action < 600)
        {
          action_color = "\033[1;95m"; // Bright Magenta for 5XX actions
        }
        else
        {
          action_color = "\033[1;31m"; // Red for very high percentage actions
        }

        std::cerr << "Player Turn: " << player_str << " | Chose Action: " << action_color << action_str << "\033[0m" << std::endl << std::endl;
      }
    }
    for (open_spiel::Player p = 0; p < bots.size(); ++p)
    {
      if (p != player)
      {
        bots[p]->InformAction(*state, player, action);
      }
    }
    history.push_back(state->ActionToString(player, action));
    state->ApplyAction(action);

    if (!quiet)
    {
      std::cerr << state->ToString() << std::endl;
      std::cerr << "Please press enter to continue...";
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
  }

  std::cerr << "Returns: " << absl::StrJoin(state->Returns(), ",")
            << " Game actions: " << absl::StrJoin(history, " ") << std::endl;
  return {state->Returns(), history};
}

// Example code for using MCTS agent to play a game
int main(int argc, char **argv)
{
  std::vector<char *> positional_args = absl::ParseCommandLine(argc, argv);
  std::mt19937 rng(Seed()); // Random number generator.

  // Create the game
  std::string game_name = absl::GetFlag(FLAGS_game);
  std::cerr << "game: " << game_name << std::endl;
  std::shared_ptr<const open_spiel::Game> game =
      open_spiel::LoadGame(game_name);

  std::cerr << "\n\033[1;37mGame Entities and Descriptions:\033[0m\n";
  std::cerr << "\033[1;37mEmpty\033[0m: Represents an empty cell.\n";
  std::cerr << "\033[1;34mW\033[0m: Represents the creature 'Wizard'.\n";
  std::cerr << "\033[1;32mR\033[0m: Represents the creature 'Rocky'.\n";
  std::cerr << "\033[1;33mA\033[0m: Represents the creature 'Arca'.\n";
  std::cerr << "\033[1;31mK\033[0m: Represents the creature 'King'.\n";
  std::cerr << "\033[1;35mM\033[0m: Represents the creature 'Mory'.\n";
  std::cerr << "\033[1;36mG\033[0m: Represents the creature 'Galio'.\n";
  std::cerr << "\033[1;91mP\033[0m: Represents a land type 'Plain'.\n";
  std::cerr << "\033[1;92mU\033[0m: Represents a spell 'Upgrade'.\n";
  std::cerr << "\033[1;93mB\033[0m: Represents a permanent spell 'Blessing'.\n";
  std::cerr << "\033[1;94mC\033[0m: Represents a permanent debuff 'Curse'.\n";
  std::cerr << "--------------------------------------\n";

  // MCTS supports arbitrary number of players, but this example assumes
  // 2-player games.
  SPIEL_CHECK_TRUE(game->NumPlayers() <= 2);

  auto evaluator =
      std::make_shared<open_spiel::algorithms::RandomRolloutEvaluator>(
          absl::GetFlag(FLAGS_rollout_count), Seed());

  std::vector<std::unique_ptr<open_spiel::Bot>> bots;
  bots.push_back(InitBot(absl::GetFlag(FLAGS_player1), *game, 0, evaluator));
  bots.push_back(InitBot(absl::GetFlag(FLAGS_player2), *game, 1, evaluator));

  std::vector<std::string> initial_actions;
  for (int i = 1; i < positional_args.size(); ++i)
  {
    initial_actions.push_back(positional_args[i]);
  }

  absl::btree_map<std::string, int> histories;
  std::vector<double> overall_returns(2, 0);
  std::vector<int> overall_wins(2, 0);
  int num_games = absl::GetFlag(FLAGS_num_games);
  for (int game_num = 0; game_num < num_games; ++game_num)
  {
    auto [returns, history] = PlayGame(*game, bots, rng, initial_actions);
    histories[absl::StrJoin(history, " ")] += 1;
    for (int i = 0; i < returns.size(); ++i)
    {
      double v = returns[i];
      overall_returns[i] += v;
      if (v > 0)
      {
        overall_wins[i] += 1;
      }
    }
  }

  std::cerr << "Number of games played: " << num_games << std::endl;
  std::cerr << "Number of distinct games played: " << histories.size()
            << std::endl;
  std::cerr << "Overall wins: " << absl::StrJoin(overall_wins, ",")
            << std::endl;
  std::cerr << "Overall returns: " << absl::StrJoin(overall_returns, ",")
            << std::endl;

  return 0;
}
