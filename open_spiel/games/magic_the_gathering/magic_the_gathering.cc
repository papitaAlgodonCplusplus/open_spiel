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
      const GameType kGameType{
          "magic_the_gathering",
          "Magic the Gathering",
          GameType::Dynamics::kSequential,
          GameType::ChanceMode::kDeterministic,
          GameType::Information::kPerfectInformation,
          GameType::Utility::kZeroSum,
          GameType::RewardModel::kTerminal,
          2,
          2,
          true,
          false,
          true,
          true,
          {}};

      std::shared_ptr<const Game> Factory(const GameParameters &params)
      {
        return std::shared_ptr<const Game>(new MagicTheGatheringGame(params));
      }

      REGISTER_SPIEL_GAME(kGameType, Factory);

      RegisterSingleTensorObserver single_tensor(kGameType.short_name);

    }

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
              std::cin.clear();
              std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
              std::cout << "Invalid input. Please enter a valid number.\n";
            }
          } while (userChoice < 0 || userChoice >= static_cast<int>(CellState::kCount));
          arena_[i] = static_cast<CellState>(userChoice);
        }
      }

      if (move < 0 || move > 506)
      {

        SpielFatalError("Unknown action: " + std::to_string(move));
        return;
      }

      if (move == 200)
      {
      }

      else if (move <= 6)
      {

        CellState selected_card = (current_player_ == 0 && move <= 6) ? arena_[move] : enemy_hand_[move];

        PlaceCardFromHand(move, selected_card);
        move_number_++;
        return;
      }

      else if (move == 7)
      {

        HandleAttack(move);
        return;
      }

      else if (move >= 8 && move <= 25)
      {

        if (move == 25)
        {

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

      else if (move >= 26 && move <= 58)
      {

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

      else if (move >= 500 && move <= 506)
      {

        current_player_ == 0 ? arena_[move - 500] = CellState::kEmpty : enemy_hand_[move - 500] = CellState::kEmpty;
      }

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

        if (has_modifications_[i] > 0)
        {
          has_modifications_[i] -= 1;
        }
        else
        {
          RestoreCreatureStats(i, start_creatures_player_0);
        }

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

      bool isEnemy = cardIndex >= (26 + kNumCols);

      int index = cardIndex + 6;

      if (def_power != -1 || def_toughness != -1)
      {

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

        if (!isEnemy)
        {

          power_[index % kNumCols] += buffValue;
          toughness_[index % kNumCols] += buffValue;
          has_modifications_[index % kNumCols] = duration;
        }
        else
        {

          enemy_power_[index % kNumCols] += buffValue;
          enemy_toughness_[index % kNumCols] += buffValue;
          enemy_has_modifications_[index % kNumCols] = duration;
        }
      }

      is_selecting_spell_target_ = -1;

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

        if (selected_card == CellState::kPermanentSpell || selected_card == CellState::kPermanentDebuff)
        {

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

      for (size_t i = start; i < end; ++i)
      {

        if (IsCreature(arena_[i]) && !sickness_system_[i])
        {
          auto it = std::find(current_attackers_.begin(), current_attackers_.end(), 0);

          if (it != current_attackers_.end())
          {
            *it = i;
          }
        }
        else
        {
          current_attackers_[i % kNumCols] = 0;
        }
      }

      for (size_t i = 0; i < current_attackers_.size(); ++i)
      {
        current_attackers_[i] > start_creatures_player_1 + kNumCols ? current_attackers_[i] = 0 : current_attackers_[i];
      }

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

      for (const auto &defender : defenders)
      {
      }

      return defenders;
    }

    void MagicTheGatheringState::HandleAttack(int move)
    {
      current_attackers_ = GetAttackers();

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

        if (attacker_pos > start_creatures_player_1 + kNumCols || defender_pos > start_creatures_player_1 + kNumCols || attacker_pos < start_creatures_player_0 || defender_pos < start_creatures_player_0)
        {
          continue;
        }

        tapping_system_[attacker_pos] = 2;
        CalculateCombatDamageBetweenCards(attacker_pos, defender_pos);
        attacker_index++;
      }

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

        if (defending_card_life <= 0)
        {

          arena_[defender_pos] = CellState::kEmpty;
          enemy_power_[defender_pos - start_creatures_player_1] = 0;
          enemy_toughness_[defender_pos - start_creatures_player_1] = 0;
        }
        if (attacking_card_life <= 0)
        {

          arena_[attacker_pos] = CellState::kEmpty;
          power_[attacker_pos - start_creatures_player_0] = 0;
          toughness_[attacker_pos - start_creatures_player_0] = 0;
        }
        break;

      case 0:
        defending_card_life = toughness_[defender_pos - start_creatures_player_0] - enemy_power_[attacker_pos - start_creatures_player_1];
        attacking_card_life = enemy_toughness_[attacker_pos - start_creatures_player_1] - power_[defender_pos - start_creatures_player_0];

        if (defending_card_life <= 0)
        {

          arena_[defender_pos] = CellState::kEmpty;
          power_[defender_pos - start_creatures_player_0] = 0;
          toughness_[defender_pos - start_creatures_player_0] = 0;
        }
        if (attacking_card_life <= 0)
        {

          arena_[attacker_pos] = CellState::kEmpty;
          enemy_power_[attacker_pos - start_creatures_player_1] = 0;
          enemy_toughness_[attacker_pos - start_creatures_player_1] = 0;
        }
        break;

      default:

        break;
      }
    }

    std::vector<Action> MagicTheGatheringState::LegalActions() const
    {
      std::vector<Action> moves;

      if (IsTerminal())
      {

        return {};
      }

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

          return std::vector<Action>{404};
        }
        return moves;
      }

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

      moves.push_back(200);

      for (int i = 0; i < 7; ++i)
      {
        switch (current_player_)
        {
        case 0:
          if (arena_[i] != CellState::kEmpty)
          {

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

            moves.push_back(i);
          }
          break;

        default:

          break;
        }
      }

      AddCreatureActions(moves);

      AddDiscardOptions(moves);

      return moves;
    }

    void MagicTheGatheringState::AddCreatureActions(std::vector<Action> &moves) const
    {
      int attack_start = (current_player_ == 0) ? start_creatures_player_0 : start_creatures_player_1;

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

      std::random_device rd;
      std::mt19937 gen(rd());
      std::uniform_int_distribution<int> dis(static_cast<int>(CellState::kPlains), static_cast<int>(CellState::kPermanentDebuff));

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

      std::fill(arena_.begin() + 7, arena_.end(), CellState::kEmpty);

      for (int i = 0; i < 7; ++i)
      {
        enemy_hand_[i] = static_cast<CellState>(dis(gen));
      }
    }

    std::string MagicTheGatheringState::ToVisualString() const
    {
      std::ostringstream str;
      str << "----------------------------------------------------------------------------\n";

      // Display the game board (5 rows, each with 15 columns)
      str << "\033[1;37m--- Game Board ---\033[0m\n";
      for (int row = 0; row < 5; ++row)
      {
        for (int col = 0; col < 15; ++col)
        {
          int index = row * 15 + col;
          str << DrawCard(arena_[index], row);
        }
        str << "\n";
      }
      str << "\n";

      // Display enemy hand
      str << "\033[1;37mEnemy Hand:\033[0m\n";
      for (int i = 0; i < 7; ++i)
      {
        str << "[" << i << "] " << DrawCard(enemy_hand_[i], 0) << " ";
      }
      str << "\n\n";

      // Bot creatures' stats
      str << "\033[1;34m-- Bot Creatures Stats --\033[0m\n";
      for (int i = 0; i < 14; ++i)
      {
        str << i << ": " << power_[i] << "/" << toughness_[i] << " | ";
      }
      str << "\n\n";

      // Enemy creatures' stats
      str << "\033[1;31m-- Enemy Creatures Stats --\033[0m\n";
      for (int i = 0; i < 14; ++i)
      {
        str << i << ": " << enemy_power_[i] << "/" << enemy_toughness_[i] << " | ";
      }
      str << "\n\n";

      // Display bot's hand
      str << "\033[1;37mBot Hand:\033[0m\n";
      for (int i = 0; i < 7; ++i)
      {
        str << "[" << i << "] " << DrawCard(enemy_hand_[i], 0) << " ";
      }
      str << "\n\n";

      // Print player and enemy lives
      str << "\033[1;37m--- Game Status ---\033[0m\n";
      str << "Player Life: \033[1;92m" << Life << "\033[0m | Enemy Life: \033[1;91m" << EnemyLife << "\033[0m\n";
      str << "----------------------------------------------------------------------------\n";

      return str.str();
    }

    // Helper function to draw a card
    std::string MagicTheGatheringState::DrawCard(const CellState &card, int row) const
    {
      switch (card)
      {
      case CellState::kEmpty:
        return "- ";
      case CellState::kPlains:
        return "\033[1;91m[P]\033[0m ";
      case CellState::kCreature1:
        return "\033[1;34m[R]\033[0m ";
      case CellState::kCreature2:
        return "\033[1;32m[W]\033[0m ";
      case CellState::kCreature3:
        return "\033[1;33m[A]\033[0m ";
      case CellState::kCreature4:
        return "\033[1;31m[K]\033[0m ";
      case CellState::kCreature5:
        return "\033[1;35m[M]\033[0m ";
      case CellState::kCreature6:
        return "\033[1;36m[G]\033[0m ";
      case CellState::kSpell:
        return "\033[1;92m[U]\033[0m ";
      case CellState::kPermanentSpell:
        return "\033[1;93m[B]\033[0m ";
      case CellState::kPermanentDebuff:
        return "\033[1;94m[C]\033[0m ";
      default:
        return "? ";
      }
    }
    // Helper function to colorize card names
    std::string MagicTheGatheringState::ColorizeCard(const CellState &card) const
    {
      switch (card)
      {
      case CellState::kCreature1:
        return "\033[1;34mR\033[0m";
      case CellState::kCreature2:
        return "\033[1;32mW\033[0m";
      case CellState::kCreature3:
        return "\033[1;33mA\033[0m";
      case CellState::kCreature4:
        return "\033[1;31mK\033[0m";
      case CellState::kCreature5:
        return "\033[1;35mM\033[0m";
      case CellState::kCreature6:
        return "\033[1;36mG\033[0m";
      case CellState::kPlains:
        return "\033[1;91mP\033[0m";
      case CellState::kSpell:
        return "\033[1;92mS\033[0m";
      case CellState::kPermanentSpell:
        return "\033[1;93mB\033[0m";
      case CellState::kPermanentDebuff:
        return "\033[1;94mC\033[0m";
      default:
        return "-";
      }
    }

    std::string MagicTheGatheringState::ToString() const
    {
      return this->ToVisualString();

      /*
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
      */
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

  }
}
