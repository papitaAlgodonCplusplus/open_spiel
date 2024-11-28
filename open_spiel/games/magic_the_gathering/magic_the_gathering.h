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

#ifndef OPEN_SPIEL_GAMES_MAGIC_THE_GATHERING_H_
#define OPEN_SPIEL_GAMES_MAGIC_THE_GATHERING_H_

#include <array>
#include <map>
#include <memory>
#include <string>
#include <vector>
#include "open_spiel/spiel.h"

namespace open_spiel
{
  namespace magic_the_gathering
  {

    // Constants.
    inline constexpr int kNumPlayers = 2;
    inline constexpr int kNumRows = 5;                    // Hand, Arena, Enemy Arena (excluded numerical info)
    inline constexpr int kNumCols = 16;                   // Columns representing positions in each zone
    inline constexpr int kNumCells = kNumRows * kNumCols; // Total number of cells in the game matrix
    inline constexpr int start_lands_player_0 = 16;
    inline constexpr int start_lands_player_1 = 64;
    inline constexpr int start_creatures_player_0 = 32;
    inline constexpr int start_creatures_player_1 = 48;

    bool is_simulated_ = true;
    int user_option = -1;

    // State of a cell representing a card or an empty slot.
    enum class CellState
    {
      kEmpty = 0,
      kPlains = 1,
      kCreature1 = 2,
      kCreature2 = 3,
      kCreature3 = 4,
      kCreature4 = 5,
      kCreature5 = 6,
      kCreature6 = 7,
      kSpell = 8,
      kPermanentSpell = 9,
      kPermanentDebuff = 10,
      kCount
    };

    // State of creature's power and toughness
    enum class CreatureState
    {
      kCreature1PlainCost = 2,
      kCreature1P = 2,
      kCreature1T = 2,
      kCreature2PlainCost = 3,
      kCreature2P = 3,
      kCreature2T = 1,
      kCreature3PlainCost = 5,
      kCreature3P = 2,
      kCreature3T = 3,
      kCreature4PlainCost = 1,
      kCreature4P = 1,
      kCreature4T = 1,
      kCreature5PlainCost = 4,
      kCreature5P = 3,
      kCreature5T = 4,
      kCreature6PlainCost = 5,
      kCreature6P = 5,
      kCreature6T = 5,
    };

    enum class SpellState
    {
      kSpellBonus = 2,
      kPermanentSpellBonus = 2,
      kPermanentDebuffP = 0,
      kPermanentDebuffT = 0,
    };

    inline constexpr int kCellStates = static_cast<int>(CellState::kCount);

    // State of an in-play game.
    class MagicTheGatheringState : public State
    {
    public:
      MagicTheGatheringState(std::shared_ptr<const Game> game);

      MagicTheGatheringState(const MagicTheGatheringState &) = default;
      MagicTheGatheringState &operator=(const MagicTheGatheringState &) = default;

      Player CurrentPlayer() const override
      {
        return IsTerminal() ? kTerminalPlayerId : current_player_;
      }
      std::string ActionToString(Player player, Action action_id) const override;
      std::string ToString() const override;
      std::string ToVisualString() const;
      std::string DrawCard(const CellState &card, int row) const;
      std::string ColorizeCard(const CellState &card) const;
      bool IsTerminal() const override;
      std::vector<double> Returns() const override;
      std::string InformationStateString(Player player) const override;
      std::string ObservationString(Player player) const override;
      void ObservationTensor(Player player,
                             absl::Span<float> values) const override;
      std::unique_ptr<State> Clone() const override;
      void UndoAction(Player player, Action move) override;
      std::vector<Action> LegalActions() const override;
      CellState BoardAt(int row, int column) const { return arena_[row * kNumCols + column]; }

      // Only used by advanced MTG gameplay scenarios.
      void SetCurrentPlayer(Player player) { current_player_ = player; }

    protected:
      std::array<CellState, kNumCells> arena_;
      std::array<int, kNumCells> tapping_system_;
      std::array<bool, kNumCells> sickness_system_;
      std::array<CellState, kNumCols> enemy_hand_;
      std::array<int, kNumCols> power_;
      std::array<int, kNumCols> toughness_;
      std::array<int, kNumCols> enemy_power_;
      std::array<int, kNumCols> enemy_toughness_;
      std::array<int, kNumCols> has_modifications_;
      std::array<int, kNumCols> enemy_has_modifications_;
      std::array<size_t, kNumCols> current_attackers_;
      std::array<size_t, kNumCols> current_defenders_;
      bool plain_placed = false;
      int Life = 20;
      int EnemyLife = 20;
      void DoApplyAction(Action move) override;
      void PlaceCardFromHand(int move, CellState selected_card);
      void SetCreatureStats(size_t index, CellState card);
      void HandleAttack(int move);
      void ExecuteCombat(std::array<size_t, kNumCols> &attackers, std::array<size_t, kNumCols> &defenders);
      void DrawRandomCard();
      void CalculateCombatDamageBetweenCards(size_t attacker_pos, size_t defender_pos);
      std::array<size_t, kNumCols> GetAttackers();
      std::array<size_t, kNumCols> GetDefenders() const;
      void ApplyModification(int cardIndex, int buffValue, int duration, int def_power = -1, int def_toughness = -1);
      void RestoreCreatureStats(int index, int arena_offset);
      void Cleanup();
      void AddCreatureActions(std::vector<Action> &moves) const;
      void AddDiscardOptions(std::vector<Action> &moves) const;
      bool HasEnoughMana(CellState selected_card) const;
      int FindFirstCreature() const;
      void HandleDefense(int move);
      std::vector<size_t> CountPlains() const;

    private:
      bool WonGame(Player player) const;
      Player current_player_ = 0;
      Player is_selecting_spell_target_ = -1;
      Player is_attacking_ = -1;
      int new_value = -1;
      int new_power = -1;
      int new_toughness = -1;
      int spell_duration = -1;
      Player outcome_ = kInvalidPlayer;
      int num_moves_ = 0;
    };

    // Game object.
    class MagicTheGatheringGame : public Game
    {
    public:
      explicit MagicTheGatheringGame(const GameParameters &params);
      int NumDistinctActions() const override { return kNumCells; }
      std::unique_ptr<State> NewInitialState() const override
      {
        return std::unique_ptr<State>(new MagicTheGatheringState(shared_from_this()));
      }
      int NumPlayers() const override { return kNumPlayers; }
      double MinUtility() const override { return -1; }
      absl::optional<double> UtilitySum() const override { return 0; }
      double MaxUtility() const override { return 1; }
      std::vector<int> ObservationTensorShape() const override
      {
        return {static_cast<int>(CellState::kPermanentDebuff) + 1, kNumRows + 4, kNumCols};
      }
      int MaxGameLength() const override { return kNumCells; }
      std::string ActionToString(Player player, Action action_id) const override;
    };

    std::string StateToString(CellState state);

    inline std::ostream &operator<<(std::ostream &stream, const CellState &state)
    {
      return stream << StateToString(state);
    }

  } // namespace magic_the_gathering
} // namespace open_spiel

#endif // OPEN_SPIEL_GAMES_MAGIC_THE_GATHERING_H_