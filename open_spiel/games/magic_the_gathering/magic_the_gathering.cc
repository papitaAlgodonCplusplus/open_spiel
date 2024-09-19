// Copyright 2019 DeepMind Technologies Limited
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

#include "open_spiel/games/magic_the_gathering/magic_the_gathering.h"

#include <algorithm>
#include <memory>
#include <utility>
#include <vector>
#include <random>
#include <algorithm>

#include "open_spiel/spiel_utils.h"
#include "open_spiel/utils/tensor_view.h"

namespace open_spiel
{
  namespace magic_the_gathering
  {
    namespace
    {

      // Facts about the game.
      const GameType kGameType{
          /*short_name=*/"magic_the_gathering",
          /*long_name=*/"Magic the Gathering",
          GameType::Dynamics::kSequential,
          GameType::ChanceMode::kDeterministic,
          GameType::Information::kPerfectInformation,
          GameType::Utility::kZeroSum,
          GameType::RewardModel::kTerminal,
          /*max_num_players=*/2,
          /*min_num_players=*/2,
          /*provides_information_state_string=*/true,
          /*provides_information_state_tensor=*/false,
          /*provides_observation_string=*/true,
          /*provides_observation_tensor=*/true,
          /*parameter_specification=*/{} // no parameters
      };

      std::shared_ptr<const Game> Factory(const GameParameters &params)
      {
        return std::shared_ptr<const Game>(new MagicTheGatheringGame(params));
      }

      REGISTER_SPIEL_GAME(kGameType, Factory);

      RegisterSingleTensorObserver single_tensor(kGameType.short_name);

    } // namespace

    std::string StateToString(CellState state)
    {
      switch (state)
      {
      case CellState::kEmpty:
        return "-";
      case CellState::kCreature1:
        return "Wizard";
      case CellState::kCreature2:
        return "Rocky";
      case CellState::kCreature3:
        return "Arca";
      case CellState::kCreature4:
        return "Puffer";
      case CellState::kCreature5:
        return "Mory";
      case CellState::kCreature6:
        return "KingRomet";
      case CellState::kPlains:
        return "Plain";
      case CellState::kSpell:
        return "Blessing";
      case CellState::kPermanentSpell:
        return "Upgrade";
      case CellState::kPermanentDebuff:
        return "Punish";
      default:
        SpielFatalError("Unknown state.");
      }
    }

    void MagicTheGatheringState::DoApplyAction(Action move)
    {
      if (!is_simulated_ && current_player_ == 1)
      {
        for (int i = 0; i < kNumCells; ++i)
        {
          int userChoice;

          std::cout << this->ToString();
          std::cout << "Choose a state for cell " << i << ":\n";
          std::cout << "0: Empty\n1: Plains\n2: Creature1\n3: Creature2\n4: Creature3\n5: Creature4\n";
          std::cout << "6: Creature5\n7: Creature6\n8: Spell\n9: Permanent Spell\n10: Permanent Debuff\n";

          do
          {
            std::cout << "Enter a number (0-" << static_cast<int>(CellState::kCount) - 1 << "): ";
            std::cin >> userChoice;

            if (std::cin.fail())
            {
              std::cin.clear();                                                   // Clear error flags
              std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignore invalid input
              std::cout << "Invalid input. Please enter a valid number.\n";
            }
          } while (userChoice < 0 || userChoice >= static_cast<int>(CellState::kCount));
          arena_[i] = static_cast<CellState>(userChoice);
        }
      }

      if (move < 0 || move > 506)
      {
        // // std::cout << "Error: Unknown action received: " << move << std::endl;
        SpielFatalError("Unknown action: " + std::to_string(move));
        return;
      }

      // // std::cout << "Processing action: " << move << " by player " << current_player_ << std::endl;
      if (move == 200)
      {
      }
      // Place a card from hand to the arena
      else if (move <= 6)
      {
        // Determine selected card from hand
        CellState selected_card = (current_player_ == 0 && move <= 6) ? arena_[move] : enemy_hand_[move];
        // // std::cout << "Selected card: " << StateToString(selected_card) << " from " << ((current_player_ == 0 && move <= 6) ? "player's hand" : "enemy's hand") << std::endl;
        // // std::cout << "Placing card " << StateToString(selected_card) << " from hand to arena at position " << move << std::endl;
        PlaceCardFromHand(move, selected_card);
        move_number_++;
        return;
      }
      // Handle attack
      else if (move == 7)
      {
        // // std::cout << "Handling attack for move: " << move << std::endl;
        HandleAttack(move);
        return;
      }
      // Handle defense
      else if (move >= 8 && move <= 25)
      {
        // // std::cout << "Handling defense for move: " << move << std::endl;
        if (move == 25)
        {
          // // std::cout << "it's time for combat\n";
          ExecuteCombat(current_attackers_, current_defenders_);
          is_attacking_ = -1;
          num_moves_++;
          Cleanup();
          WonGame(1 - current_player_) ? outcome_ = 1 - current_player_ : outcome_;
          return;
        }
        else
        {
          HandleDefense(move);
          return;
        }
      }
      // Buff card's power and toughness
      else if (move >= 26 && move <= 58)
      {
        // // std::cout << "Applying buff to card at position " << (move - 24) << std::endl;
        if (new_power != -1 || new_toughness != -1)
        {
          ApplyModification(move, new_value, spell_duration, new_power, new_toughness);
        }
        else
        {
          ApplyModification(move, new_value, spell_duration);
        }
        new_value = -1;
        new_power = -1;
        new_toughness = -1;
        spell_duration = -1;
        return;
      }
      else if (move == 404)
      {
        is_selecting_spell_target_ = -1;
      }
      // Discard card from hand (pass action)
      else if (move >= 500 && move <= 506)
      {
        // // std::cout << "Discarding card from hand at position " << (move - 500) << std::endl;
        current_player_ == 0 ? arena_[move - 500] = CellState::kEmpty : enemy_hand_[move - 500] = CellState::kEmpty;
      }

      // Cleanup buffs and dead creatures and switch turn
      // // std::cout << "Cleaning up buffs" << std::endl;
      Cleanup();

      if (WonGame(current_player_))
      {
        outcome_ = current_player_;
      }

      if (is_selecting_spell_target_ == -1)
      {
        for (size_t i = 0; i < kNumCols; i++)
        {
          bool should_draw = (current_player_ == 0 && arena_[i] == CellState::kEmpty) ||
                             (current_player_ == 1 && enemy_hand_[i] == CellState::kEmpty);

          if (should_draw)
          {
            DrawRandomCard();
            break;
          }
        }
        move_number_++;
        current_player_ = 1 - current_player_;
        // // std::cout << "Turn completed. Current move number: " << move_number_ << ". Next player: " << current_player_ << std::endl;
        // // std::cout << this->ToString() << std::endl;
      }

      return;
    }

    void MagicTheGatheringState::HandleDefense(int move)
    {
      size_t offset = current_player_ == 0 ? (start_creatures_player_0 - 8) : (start_creatures_player_1 - 8);

      for (size_t i = 0; i < current_defenders_.size(); ++i)
      {
        if (current_defenders_[i] == 0)
        {
          current_defenders_[i] = move + offset;
          break;
        }
      }
    }

    void MagicTheGatheringState::Cleanup()
    {
      plain_placed = false;
      std::fill(current_defenders_.begin(), current_defenders_.begin() + kNumCols, 0);
      std::fill(current_attackers_.begin(), current_attackers_.begin() + kNumCols, 0);

      // Cleanup tapping
      for (size_t i = 0; i < kNumCells; i++)
      {
        tapping_system_[i] == 0 ?: tapping_system_[i] -= 1;
        sickness_system_[i] = false;
      }

      for (size_t i = 0; i < kNumCols; i++)
      {
        if (arena_[i + start_creatures_player_0] == CellState::kEmpty)
        {
          power_[i] = 0;
          toughness_[i] = 0;
        }

        if (arena_[i + start_creatures_player_1] == CellState::kEmpty)
        {
          enemy_power_[i] = 0;
          enemy_toughness_[i] = 0;
        }

        // Check player's buffs
        if (has_modifications_[i] > 0)
        {
          has_modifications_[i] -= 1;
        }
        else
        {
          RestoreCreatureStats(i, start_creatures_player_0);
        }

        // Check enemy's buffs
        if (enemy_has_modifications_[i] > 0)
        {
          enemy_has_modifications_[i] -= 1;
        }
        else
        {
          (i, 29);
        }
      }
    }

    void MagicTheGatheringState::RestoreCreatureStats(int index, int arena_offset)
    {
      std::array<int, kNumCols> *power = nullptr;
      std::array<int, kNumCols> *toughness = nullptr;

      if (arena_offset == start_creatures_player_0)
      {
        power = &power_;
        toughness = &toughness_;
      }
      else
      {
        power = &enemy_power_;
        toughness = &enemy_toughness_;
      }

      // // std::cout << " reseting for " << StateToString(arena_[index + arena_offset]) << " which is " << (*power)[index] << " / " << (*toughness)[index] << std::endl;
      switch (arena_[index + arena_offset])
      {
      case CellState::kCreature1:
        (*power)[index] = static_cast<int>(CreatureState::kCreature1P);
        (*toughness)[index] = static_cast<int>(CreatureState::kCreature1T);
        break;
      case CellState::kCreature2:
        (*power)[index] = static_cast<int>(CreatureState::kCreature2P);
        (*toughness)[index] = static_cast<int>(CreatureState::kCreature2T);
        break;
      case CellState::kCreature3:
        (*power)[index] = static_cast<int>(CreatureState::kCreature3P);
        (*toughness)[index] = static_cast<int>(CreatureState::kCreature3T);
      case CellState::kCreature4:
        (*power)[index] = static_cast<int>(CreatureState::kCreature4P);
        (*toughness)[index] = static_cast<int>(CreatureState::kCreature4T);
      case CellState::kCreature5:
        (*power)[index] = static_cast<int>(CreatureState::kCreature5P);
        (*toughness)[index] = static_cast<int>(CreatureState::kCreature5T);
      case CellState::kCreature6:
        (*power)[index] = static_cast<int>(CreatureState::kCreature6P);
        (*toughness)[index] = static_cast<int>(CreatureState::kCreature6T);
        break;
      default:
        break;
      }
    }

    void MagicTheGatheringState::ApplyModification(int cardIndex, int buffValue, int duration, int def_power, int def_toughness)
    {
      // Determine if the card is an enemy card and print the result
      bool isEnemy = cardIndex >= (26 + kNumCols);
      // std::cout << "Card Index: " << cardIndex << ", Is Enemy: " << isEnemy << std::endl;

      int index = cardIndex + 6;
      // std::cout << "Calculated Index: " << index << std::endl;

      // Print the selected arrays
      // std::cout << "Target Power Array: " << (isEnemy ? "enemy_power_" : "power_") << std::endl;
      // std::cout << "Target Toughness Array: " << (isEnemy ? "enemy_toughness_" : "toughness_") << std::endl;
      // std::cout << "Target Modifications Array: " << (isEnemy ? "enemy_has_modifications_" : "has_modifications_") << std::endl;

      if (def_power != -1 || def_toughness != -1)
      {
        // Print the new values if setting specific power/toughness
        // std::cout << "Setting power and toughness directly." << std::endl;
        // std::cout << "New Power Value: " << def_power << std::endl;
        // std::cout << "New Toughness Value: " << def_toughness << std::endl;

        if (!isEnemy)
        {
          power_[index % kNumCols] = def_power;
          toughness_[index % kNumCols] = def_toughness;
          has_modifications_[index % kNumCols] = duration;
        }
        else
        {
          enemy_power_[index % kNumCols] = def_power;
          enemy_toughness_[index % kNumCols] = def_toughness;
          enemy_has_modifications_[index % kNumCols] = duration;
        }
      }
      else
      {
        // std::cout << "Applying buff value: " << buffValue << std::endl;

        if (!isEnemy)
        {
          // std::cout << "Old Power Value: " << power_[index] << std::endl;
          // std::cout << "Old Toughness Value: " << toughness_[index] << std::endl;

          power_[index % kNumCols] += buffValue;
          toughness_[index % kNumCols] += buffValue;
          has_modifications_[index % kNumCols] = duration;
        }
        else
        {
          // std::cout << "Old Power Value: " << enemy_power_[index] << std::endl;
          // std::cout << "Old Toughness Value: " << enemy_toughness_[index] << std::endl;

          enemy_power_[index % kNumCols] += buffValue;
          enemy_toughness_[index % kNumCols] += buffValue;
          enemy_has_modifications_[index % kNumCols] = duration;
        }
      }

      // std::cout << "Modification Duration: " << duration << std::endl;

      // Reset spell target selection and print the state
      is_selecting_spell_target_ = -1;
      // std::cout << "Reset is_selecting_spell_target_ to: " << is_selecting_spell_target_ << std::endl;

      return;
    }

    bool IsCreature(CellState cell)
    {
      switch (cell)
      {
      case CellState::kCreature1:
      case CellState::kCreature2:
      case CellState::kCreature3:
      case CellState::kCreature4:
      case CellState::kCreature5:
      case CellState::kCreature6:
        return true;
      default:
        return false;
      }
    }

    bool IsLand(CellState cell)
    {
      switch (cell)
      {
      case CellState::kPlains:
        return true;
      default:
        return false;
      }
    }

    bool IsSpell(CellState cell)
    {
      switch (cell)
      {
      case CellState::kSpell:
      case CellState::kPermanentSpell:
      case CellState::kPermanentDebuff:
        return true;
      default:
        return false;
      }
    }

    void MagicTheGatheringState::DrawRandomCard()
    {
      std::random_device rd;
      std::mt19937 gen(rd());
      std::uniform_int_distribution<int> dis(static_cast<int>(CellState::kPlains), static_cast<int>(CellState::kPermanentDebuff));
      if (current_player_ == 0)
      {
        for (size_t i = 0; i < 7; ++i)
        {
          if (arena_[i] == CellState::kEmpty)
          {
            if (!is_simulated_)
            {
              int new_card;
              std::cout << "Choose new card bot\n";
              std::cout << this->ToString();
              std::cin >> new_card;
              arena_[i] = static_cast<CellState>(new_card);
              break;
            }
            arena_[i] = static_cast<CellState>(dis(gen));
            // // std::cout << i << " in hand is now " << StateToString(arena_[i]) << std::endl;
            break;
          }
        }
      }
      else
      {
        for (size_t i = 0; i < 7; ++i)
        {
          if (enemy_hand_[i] == CellState::kEmpty)
          {
            if (!is_simulated_)
            {
              int new_card;
              std::cout << "Choose new card for enemy\n";
              std::cout << this->ToString();
              std::cin >> new_card;
              enemy_hand_[i] = static_cast<CellState>(new_card);
              break;
            }
            enemy_hand_[i] = static_cast<CellState>(dis(gen));
            // // std::cout << i << " in hand is now " << StateToString(arena_[i]) << std::endl;
            break;
          }
        }
      }
    }

    bool MagicTheGatheringState::HasEnoughMana(CellState selected_card) const
    {
      std::vector<size_t> untapped_plains = CountPlains();
      switch (selected_card)
      {
      case CellState::kCreature1:
        return untapped_plains.size() >= static_cast<size_t>(CreatureState::kCreature1PlainCost);

      case CellState::kCreature2:
        return untapped_plains.size() >= static_cast<size_t>(CreatureState::kCreature2PlainCost);

      case CellState::kCreature3:
        return untapped_plains.size() >= static_cast<size_t>(CreatureState::kCreature3PlainCost);

      case CellState::kCreature4:
        return untapped_plains.size() >= static_cast<size_t>(CreatureState::kCreature4PlainCost);

      case CellState::kCreature5:
        return untapped_plains.size() >= static_cast<size_t>(CreatureState::kCreature5PlainCost);

      case CellState::kCreature6:
        return untapped_plains.size() >= static_cast<size_t>(CreatureState::kCreature6PlainCost);
      default:
        return false;
      }
    }

    std::vector<size_t> MagicTheGatheringState::CountPlains() const
    {
      std::vector<size_t> plains_positions;
      size_t start = (current_player_ == 0) ? start_lands_player_0 : start_lands_player_1;
      size_t end = start + kNumCols;

      for (size_t i = start; i < end; i++)
      {
        if (arena_[i] != CellState::kEmpty && tapping_system_[i] == 0)
        {
          plains_positions.push_back(i);
        }
      }

      return plains_positions;
    }

    void MagicTheGatheringState::PlaceCardFromHand(int move, CellState selected_card)
    {
      size_t start_plains = (current_player_ == 0) ? start_lands_player_0 : start_lands_player_1;
      size_t end_plains = start_plains + kNumCols;

      size_t start_creatures = (current_player_ == 0) ? start_creatures_player_0 : start_creatures_player_1;
      size_t end_creatures = start_creatures + kNumCols;
      current_player_ == 0 ? arena_[move] = CellState::kEmpty : enemy_hand_[move] = CellState::kEmpty;
      if (IsLand(selected_card))
      {
        for (size_t i = start_plains; i < end_plains; ++i)
        {
          if (arena_[i] == CellState::kEmpty)
          {
            arena_[i] = selected_card;
            break;
          }
        }
        plain_placed = true;
      }
      else if (IsCreature(selected_card))
      {
        for (size_t i = start_creatures; i < end_creatures; ++i)
        {
          if (arena_[i] == CellState::kEmpty)
          {
            std::vector<size_t> untapped_plains = CountPlains();
            arena_[i] = selected_card;
            sickness_system_[i] = true;
            for (size_t position : untapped_plains)
            {
              tapping_system_[position] = 1;
            }
            SetCreatureStats(i, selected_card);
            break;
          }
        }
      }
      else if (IsSpell(selected_card))
      {
        is_selecting_spell_target_ = current_player_;
        // Is a permanent
        if (selected_card == CellState::kPermanentSpell || selected_card == CellState::kPermanentDebuff)
        {
          // Has default buffing/debuffing values
          if (selected_card == CellState::kPermanentDebuff)
          {
            new_power = static_cast<int>(SpellState::kPermanentDebuffP);
            new_toughness = static_cast<int>(SpellState::kPermanentDebuffT);
          }
          else
          {
            new_value = static_cast<int>(SpellState::kPermanentSpellBonus);
          }
          spell_duration = 99999;
        }
        else
        {
          new_value = static_cast<int>(SpellState::kSpellBonus);
          spell_duration = 1;
        }
      }
      return;
    }

    // Function to get the power of a creature given its position or type.
    int GetCreaturePower(CellState creature)
    {
      switch (creature)
      {
      case CellState::kCreature1:
        return static_cast<int>(CreatureState::kCreature1P);
      case CellState::kCreature2:
        return static_cast<int>(CreatureState::kCreature2P);
      case CellState::kCreature3:
        return static_cast<int>(CreatureState::kCreature3P);
      case CellState::kCreature4:
        return static_cast<int>(CreatureState::kCreature4P);
      case CellState::kCreature5:
        return static_cast<int>(CreatureState::kCreature5P);
      case CellState::kCreature6:
        return static_cast<int>(CreatureState::kCreature6P);
      default:
        return 0;
      }
    }

    // Function to get the power of a creature given its position or type.
    int GetCreatureToughness(CellState creature)
    {
      switch (creature)
      {
      case CellState::kCreature1:
        return static_cast<int>(CreatureState::kCreature1T);
      case CellState::kCreature2:
        return static_cast<int>(CreatureState::kCreature2T);
      case CellState::kCreature3:
        return static_cast<int>(CreatureState::kCreature3T);
      case CellState::kCreature4:
        return static_cast<int>(CreatureState::kCreature4T);
      case CellState::kCreature5:
        return static_cast<int>(CreatureState::kCreature5T);
      case CellState::kCreature6:
        return static_cast<int>(CreatureState::kCreature6T);
      default:
        return 0;
      }
    }

    void MagicTheGatheringState::SetCreatureStats(size_t index, CellState card)
    {
      int power = GetCreaturePower(card);
      int toughness = GetCreatureToughness(card);
      // // std::cout << " Setting " << index << " as " << power << " and " << toughness << std::endl;
      if (current_player_ == 0)
      {
        power_[index % start_creatures_player_0] = power;
        toughness_[index % start_creatures_player_0] = toughness;
      }
      else
      {
        enemy_power_[index % start_creatures_player_1] = power;
        enemy_toughness_[index % start_creatures_player_1] = toughness;
      }
    }

    // Function to find the first creature in a list of cells
    int MagicTheGatheringState::FindFirstCreature() const
    {
      size_t start = (current_player_ == 0) ? start_creatures_player_0 : start_creatures_player_1;
      for (size_t i = start; i < start + kNumCols; ++i)
      {
        if (IsCreature(arena_[i]))
        {
          return i;
        }
      }
      return -1;
    }

    std::array<size_t, kNumCols> MagicTheGatheringState::GetAttackers()
    {
      size_t start = (current_player_ == 0) ? start_creatures_player_0 : start_creatures_player_1;
      size_t end = start + kNumCols;

      // Debugging: Print the start and end values
      // // std::cout << "Start: " << start << ", End: " << end << std::endl;

      for (size_t i = start; i < end; ++i)
      {
        // Debugging: Print the current index (i) and the creature/sickness status
        // // std::cout << "Index: " << i << ", IsCreature: " << StateToString(arena_[i]) << ", Sickness: " << sickness_system_[i] << std::endl;

        if (IsCreature(arena_[i]) && !sickness_system_[i])
        {
          auto it = std::find(current_attackers_.begin(), current_attackers_.end(), 0);

          if (it != current_attackers_.end())
          {
            *it = i;
            // // std::cout << "Attacker added at position: " << std::distance(current_attackers_.begin(), it) << " with value: " << i << std::endl;
          }
        }
        else
        {
          current_attackers_[i % kNumCols] = 0;
        }
      }

      // // std::cout << "current_attackers_ has MASSIVE : " << current_attackers_.size() << std::endl;

      // Debugging: Print the final list of attackerss
      // // std::cout << "Attackers: ";
      for (size_t i = 0; i < current_attackers_.size(); ++i)
      {
        current_attackers_[i] > start_creatures_player_1 + kNumCols ? current_attackers_[i] = 0 : current_attackers_[i];
        // // std::cout << current_attackers_[i] << " ";
      }
      // // std::cout << std::endl;

      return current_attackers_;
    }

    std::array<size_t, kNumCols> MagicTheGatheringState::GetDefenders() const
    {
      std::array<size_t, kNumCols> defenders = {};
      size_t start = (current_player_ == 0) ? start_creatures_player_0 : start_creatures_player_1;
      size_t end = start + kNumCols;

      size_t defender_index = 0;
      for (size_t i = start; i < end; ++i)
      {
        IsCreature(arena_[i]) && tapping_system_[i] == 0 ? defenders[defender_index] = i : defenders[defender_index] = 0;
        ++defender_index;
      }

      // Debugging: Print the final list of attackerss
      // // std::cout << "Defenders Possible: ";
      for (const auto &defender : defenders)
      {
        // // std::cout << defender << " ";
      }
      // // std::cout << std::endl;
      return defenders;
    }

    void MagicTheGatheringState::HandleAttack(int move)
    {
      current_attackers_ = GetAttackers();
      // // std::cout << current_player_ << " has " << current_attackers_.size() << " attackers" << "\n";
      is_attacking_ = current_player_;
      current_player_ = 1 - current_player_;
      return;
    }

    void MagicTheGatheringState::ExecuteCombat(std::array<size_t, kNumCols> &attackers, std::array<size_t, kNumCols> &defenders)
    {
      size_t attacker_index = 0;
      for (size_t i = 0; i < defenders.size() && attacker_index < attackers.size(); i++)
      {
        size_t defender_pos = defenders[i];
        size_t attacker_pos = attackers[attacker_index];
        // // std::cout << "Battle between: " << attacker_pos << " and " << defender_pos << " at " << i << std::endl;
        if (attacker_pos > start_creatures_player_1 + kNumCols || defender_pos > start_creatures_player_1 + kNumCols || attacker_pos < start_creatures_player_0 || defender_pos < start_creatures_player_0)
        {
          continue;
        }

        tapping_system_[attacker_pos] = 2;
        CalculateCombatDamageBetweenCards(attacker_pos, defender_pos);
        attacker_index++;
      }
      // Remaining attackers deal direct damage
      while (attacker_index < attackers.size())
      {
        if (attackers[attacker_index] < start_creatures_player_0 || attackers[attacker_index] > start_creatures_player_1 + kNumCols)
        {
          attacker_index++;
          continue;
        }

        size_t attacker_id = attackers[attacker_index];
        size_t power_index = attacker_id - (current_player_ == 1 ? start_creatures_player_0 : start_creatures_player_1);
        if (current_player_ == 1)
        {
          EnemyLife -= power_[power_index];
        }
        else
        {
          Life -= enemy_power_[power_index];
        }

        tapping_system_[attacker_id] = 2;
        attacker_index++;
      }
    }

    void MagicTheGatheringState::CalculateCombatDamageBetweenCards(size_t attacker_pos, size_t defender_pos)
    {
      int defending_card_life = 0;
      int attacking_card_life = 0;

      switch (current_player_)
      {
      case 1:
        defending_card_life = enemy_toughness_[defender_pos - start_creatures_player_1] - power_[attacker_pos - start_creatures_player_0];
        attacking_card_life = toughness_[attacker_pos - start_creatures_player_0] - enemy_power_[defender_pos - start_creatures_player_1];

        // // std::cout << "Defender's toughness: " << enemy_toughness_[defender_pos - 29] << ", Attacker's power: " << power_[attacker_pos - 16] << " defender creature is " << arena_[defender_pos] << " attacker creature is " << arena_[attacker_pos] << "\n";
        // // std::cout << "Attacker's toughness: " << toughness_[attacker_pos - 16] << ", Defender's power: " << enemy_power_[defender_pos - 29] << "\n";

        if (defending_card_life <= 0)
        {
          // // std::cout << "Defender's card at position " << defender_pos << " is destroyed.\n";
          arena_[defender_pos] = CellState::kEmpty;
          enemy_power_[defender_pos - start_creatures_player_1] = 0;
          enemy_toughness_[defender_pos - start_creatures_player_1] = 0;
        }
        if (attacking_card_life <= 0)
        {
          // // std::cout << "Attacker's card at position " << attacker_pos << " is destroyed.\n";
          arena_[attacker_pos] = CellState::kEmpty;
          power_[attacker_pos - start_creatures_player_0] = 0;
          toughness_[attacker_pos - start_creatures_player_0] = 0;
        }
        break;

      case 0:
        defending_card_life = toughness_[defender_pos - start_creatures_player_0] - enemy_power_[attacker_pos - start_creatures_player_1];
        attacking_card_life = enemy_toughness_[attacker_pos - start_creatures_player_1] - power_[defender_pos - start_creatures_player_0];

        // // std::cout << "Defender's toughness: " << toughness_[defender_pos - 16] << ", Attacker's power: " << enemy_power_[attacker_pos - 29] << "\n";
        // // std::cout << "Attacker's toughness: " << enemy_toughness_[attacker_pos - 29] << ", Defender's power: " << power_[defender_pos - 16] << "\n";

        if (defending_card_life <= 0)
        {
          // // std::cout << "Defender's card at position " << defender_pos << " is destroyed.\n";
          arena_[defender_pos] = CellState::kEmpty;
          power_[defender_pos - start_creatures_player_0] = 0;
          toughness_[defender_pos - start_creatures_player_0] = 0;
        }
        if (attacking_card_life <= 0)
        {
          // // std::cout << "Attacker's card at position " << attacker_pos << " is destroyed.\n";
          arena_[attacker_pos] = CellState::kEmpty;
          enemy_power_[attacker_pos - start_creatures_player_1] = 0;
          enemy_toughness_[attacker_pos - start_creatures_player_1] = 0;
        }
        break;

      default:
        // // std::cout << "Invalid player case.\n";
        break;
      }
    }

    std::vector<Action> MagicTheGatheringState::LegalActions() const
    {
      std::vector<Action> moves;

      // // std::cout << "Checking if the state is terminal.\n";
      if (IsTerminal())
      {
        // // std::cout << "State is terminal, returning no actions.\n";
        return {};
      }

      // // std::cout << "Is selecting spell target: " << is_selecting_spell_target_ << "\n";
      if (is_selecting_spell_target_ != -1)
      {
        int offset = 6;

        for (int i = start_creatures_player_0; i < start_creatures_player_0 + kNumCols; ++i)
        {
          if (arena_[i] != CellState::kEmpty)
          {
            moves.push_back(i - offset);
          }
        }

        for (int i = start_creatures_player_1; i < start_creatures_player_1 + kNumCols; ++i)
        {
          if (arena_[i] != CellState::kEmpty)
          {
            moves.push_back(i - offset);
          }
        }

        if (moves.empty())
        {
          // // std::cout << "No valid spell target found, returning action 404.\n";
          return std::vector<Action>{404};
        }
        return moves;
      }

      // // std::cout << is_attacking_ << " is attacking - current player " << current_player_ << "\n";
      if (is_attacking_ != -1)
      {
        std::array<size_t, kNumCols> defenders = GetDefenders();
        int offset = (is_attacking_ == 1) ? 24 : 40;
        for (size_t i = 0; i < defenders.size(); i++)
        {
          if (std::find(current_defenders_.begin(), current_defenders_.end(), defenders[i]) != current_defenders_.end())
          {
            continue;
          }
          else
          {
            // // std::cout << " Defenders at i " << defenders[i] << " i " << i << " offset " << offset << " because " << defenders[i] << " not at current_defenders_" << std::endl;
            moves.push_back(defenders[i] - offset);
          }
        }

        if (moves.empty())
        {
          return std::vector<Action>{25};
        }
        else
        {
          moves.push_back(25);
          return moves;
        }
      }

      // Skip Turn
      moves.push_back(200);

      // // std::cout << "Current player: " << current_player_ << "\n";
      for (int i = 0; i < 7; ++i)
      {
        switch (current_player_)
        {
        case 0:
          if (arena_[i] != CellState::kEmpty)
          {
            // // std::cout << "Player 0: Adding move for card at position: " << i << "\n";
            if (IsCreature(arena_[i]) && !HasEnoughMana(arena_[i]))
            {
              continue;
            }
            if (IsSpell(arena_[i]) && (FindFirstCreature() == -1))
            {
              continue;
            }
            if (IsLand(arena_[i]) && plain_placed)
            {
              continue;
            }
            moves.push_back(i);
          }
          break;

        case 1:
          if (enemy_hand_[i] != CellState::kEmpty)
          {
            if (IsCreature(enemy_hand_[i]) && !HasEnoughMana(enemy_hand_[i]))
            {
              continue;
            }
            if (IsSpell(enemy_hand_[i]) && (FindFirstCreature() == -1))
            {
              continue;
            }
            if (IsLand(enemy_hand_[i]) && plain_placed)
            {
              continue;
            }
            // // std::cout << "Player 1: Adding move for card at position: " << i << "\n";
            moves.push_back(i);
          }
          break;

        default:
          // // std::cout << "Invalid player case.\n";
          break;
        }
      }

      // // std::cout << "Adding creature actions.\n";
      AddCreatureActions(moves);

      // // std::cout << "Adding discard options.\n";
      AddDiscardOptions(moves);

      return moves;
    }

    void MagicTheGatheringState::AddCreatureActions(std::vector<Action> &moves) const
    {
      int attack_start = (current_player_ == 0) ? start_creatures_player_0 : start_creatures_player_1;

      // Add attacking moves
      for (int i = attack_start; i < attack_start + kNumCols; ++i)
      {
        if (arena_[i] != CellState::kEmpty && !sickness_system_[i])
        {
          moves.push_back(7);
          break;
        }
      }
    }

    void MagicTheGatheringState::AddDiscardOptions(std::vector<Action> &moves) const
    {
      int num_hand_cards = 0;

      switch (current_player_)
      {
      case 0:
        for (size_t i = 0; i < 7; i++)
        {
          if (arena_[i] != CellState::kEmpty)
          {
            num_hand_cards++;
          }
        }
        break;

      case 1:
        for (size_t i = 0; i < 7; i++)
        {
          if (enemy_hand_[i] != CellState::kEmpty)
          {
            num_hand_cards++;
          }
        }
        break;

      default:
        break;
      }

      if (num_hand_cards == 7)
      {
        for (size_t i = 0; i < 7; i++)
        {
          moves.push_back(500 + i);
        }
      }
    }

    std::string MagicTheGatheringState::ActionToString(Player player,
                                                       Action action_id) const
    {
      return game_->ActionToString(player, action_id);
    }

    bool MagicTheGatheringState::WonGame(Player player) const
    {
      switch (player)
      {
      case 0:
        return EnemyLife <= 0;

      case 1:
        return Life <= 0;

      default:
        std::string msg = "Unknown player: " + std::to_string(player);
        SpielFatalError(msg);
        break;
      }
    }

    MagicTheGatheringState::MagicTheGatheringState(std::shared_ptr<const Game> game)
        : State(game)
    {
      // Random number generator
      std::random_device rd;
      std::mt19937 gen(rd());
      std::uniform_int_distribution<int> dis(static_cast<int>(CellState::kPlains), static_cast<int>(CellState::kPermanentDebuff));

      // Fill arena_[0:6] with random CellState (Hand)

      if (user_option == -1)
      {
        std::cout << "Is it simulated? (Enter 1 for true, 0 for false): ";
        std::cin >> is_simulated_;
        user_option = is_simulated_;
      }
      if (!is_simulated_)
      {
        for (int i = 0; i < 7; ++i)
        {
          int new_card;
          std::cout << "Choose new card for bot\n";
          std::cout << this->ToString();
          std::cin >> new_card;
          arena_[i] = static_cast<CellState>(new_card);
        }
      }
      else
      {
        for (int i = 0; i < 7; ++i)
        {
          arena_[i] = static_cast<CellState>(dis(gen));
        }
      }

      // Fill arena_[7:] with CellState::kEmpty
      std::fill(arena_.begin() + 7, arena_.end(), CellState::kEmpty);

      // Fill enemy hand (hidden) with random CellState

      for (int i = 0; i < 7; ++i)
      {
        enemy_hand_[i] = static_cast<CellState>(dis(gen));
      }
    }

    std::string MagicTheGatheringState::ToString() const
    {
      std::ostringstream str;
      for (int row = 0; row < kNumRows; ++row)
      {
        for (int col = 0; col < kNumCols; ++col)
        {
          CellState state = arena_[row * kNumCols + col];
          switch (state)
          {
          case CellState::kEmpty:
            str << "- ";
            break;
          case CellState::kPlains:
            str << "P ";
            break;
          case CellState::kCreature1:
            str << "R ";
            break;
          case CellState::kCreature2:
            str << "W ";
            break;
          case CellState::kCreature3:
            str << "A ";
            break;
          case CellState::kCreature4:
            str << "K ";
            break;
          case CellState::kCreature5:
            str << "P ";
            break;
          case CellState::kCreature6:
            str << "G ";
            break;
          case CellState::kSpell:
            str << "S ";
            break;
          case CellState::kPermanentSpell:
            str << "B ";
            break;
          case CellState::kPermanentDebuff:
            str << "C ";
            break;
          default:
            str << "? ";
            break;
          }
        }
        str << std::endl;
      }
      str << "-- Bot Creatures Stats --" << std::endl;
      for (int i = 0; i < kNumCols; ++i)
      {
        str << std::to_string(power_[i]) + " ";
      }
      str << std::endl;
      for (int i = 0; i < kNumCols; ++i)
      {
        str << std::to_string(toughness_[i]) + " ";
      }
      str << std::endl;
      str << "-- Enemy Hand --" << std::endl;
      for (int row = 0; row < 7; ++row)
      {
        CellState state = enemy_hand_[row];
        switch (state)
        {
        case CellState::kEmpty:
          str << "- ";
          break;
        case CellState::kPlains:
          str << "P ";
          break;
        case CellState::kCreature1:
          str << "1 ";
          break;
        case CellState::kCreature2:
          str << "2 ";
          break;
        case CellState::kCreature3:
          str << "3 ";
          break;
        case CellState::kCreature4:
          str << "4 ";
          break;
        case CellState::kCreature5:
          str << "5 ";
          break;
        case CellState::kCreature6:
          str << "6 ";
          break;
        case CellState::kSpell:
          str << "S ";
          break;
        case CellState::kPermanentSpell:
          str << "B ";
          break;
        case CellState::kPermanentDebuff:
          str << "C ";
          break;
        default:
          str << "? ";
          break;
        }
      }
      str << std::endl;
      str << "-- Enemy Creatures Stats --" << std::endl;
      for (int i = 0; i < kNumCols; ++i)
      {
        str << std::to_string(enemy_power_[i]) + " ";
      }
      str << std::endl;
      for (int i = 0; i < kNumCols; ++i)
      {
        str << std::to_string(enemy_toughness_[i]) + " ";
      }
      str << std::endl;
      str << "Life: " << Life << " - " << EnemyLife << std::endl;
      return str.str();
    }

    bool MagicTheGatheringState::IsTerminal() const
    {
      return outcome_ != kInvalidPlayer;
    }

    std::vector<double> MagicTheGatheringState::Returns() const
    {
      if (WonGame(Player{0}))
      {
        return {1.0, -1.0};
      }
      else if (WonGame(Player{1}))
      {
        return {-1.0, 1.0};
      }
      else
      {
        return {0.0, 0.0};
      }
    }

    std::string MagicTheGatheringState::InformationStateString(Player player) const
    {
      SPIEL_CHECK_GE(player, 0);
      SPIEL_CHECK_LT(player, num_players_);
      return HistoryString();
    }

    std::string MagicTheGatheringState::ObservationString(Player player) const
    {
      SPIEL_CHECK_GE(player, 0);
      SPIEL_CHECK_LT(player, num_players_);
      return ToString();
    }

    void MagicTheGatheringState::ObservationTensor(Player player,
                                                   absl::Span<float> values) const
    {
      SPIEL_CHECK_GE(player, 0);
      SPIEL_CHECK_LT(player, num_players_);

      // Treat `values` as a 2-d tensor. While it may not be
      TensorView<2> view(values, {kCellStates, kNumCells + (kNumCols * 4)}, true);
      for (int cell = 0; cell < kNumCells; ++cell)
      {
        view[{static_cast<int>(arena_[cell]), cell}] = 1.0;
      }
      for (int cell = 0; cell < kNumCols; ++cell)
      {
        view[{static_cast<int>(power_[cell]), cell}] = 1.0;
      }
      for (int cell = 0; cell < kNumCols; ++cell)
      {
        view[{static_cast<int>(toughness_[cell]), cell}] = 1.0;
      }
      for (int cell = 0; cell < kNumCols; ++cell)
      {
        view[{static_cast<int>(enemy_power_[cell]), cell}] = 1.0;
      }
      for (int cell = 0; cell < kNumCols; ++cell)
      {
        view[{static_cast<int>(enemy_toughness_[cell]), cell}] = 1.0;
      }
    }

    void MagicTheGatheringState::UndoAction(Player player, Action move)
    {
      arena_[move] = CellState::kEmpty;
      current_player_ = player;
      outcome_ = kInvalidPlayer;
      num_moves_ -= 1;
      history_.pop_back();
      --move_number_;
    }

    std::unique_ptr<State> MagicTheGatheringState::Clone() const
    {
      return std::unique_ptr<State>(new MagicTheGatheringState(*this));
    }

    std::string MagicTheGatheringGame::ActionToString(Player player,
                                                      Action action_id) const
    {
      return player == 0 ? "Bot - " + std::to_string(action_id) : "Enemy - " + std::to_string(action_id);
    }

    MagicTheGatheringGame::MagicTheGatheringGame(const GameParameters &params)
        : Game(kGameType, params) {}

  } // namespace magic_the_gathering
} // namespace open_spiel
